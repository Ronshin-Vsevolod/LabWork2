/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "LevelManager.h"
#include "Player.h"
#include "GameManager.h"
#include "EnemyFactory.h"
#include <algorithm>
#include <iostream>
#include <string>

LevelManager::LevelManager(const LevelData* levelData)
    : currentLevel(levelData),
      spawning(true),
      lastSpawnTurn(0),
      currentWaveIndex(0),
      maxEnemies(0)
{
    if (currentLevel)
    {
        occupiedCells.resize(currentLevel->fieldSize, false);
        maxEnemies = static_cast<int>(currentLevel->fieldSize * 0.75);
    }
    else
    {
        occupiedCells.resize(0, false);
    }
}

LevelData* LevelManager::getLevelData()
{
    return const_cast<LevelData*>(currentLevel);
}

void LevelManager::placePlayer(Player* newPlayer)
{
    if (newPlayer && currentLevel)
    {
        newPlayer->location = currentLevel->fieldSize / 2;
        updateOccupiedCells(*newPlayer);
        std::cout << "Player placed in position " << newPlayer->location << "\n";
    }
}

void LevelManager::removeDeadEnemies(const Player& player)
{
    enemies.erase
    (
        std::remove_if
        (
            enemies.begin(),
            enemies.end(),
            [](const std::shared_ptr<Enemy>& enemy)
            {
                return enemy->health <= 0;
            }
        ),
        enemies.end()
    );
    updateOccupiedCells(player);
}

const std::vector<std::shared_ptr<Enemy>>& LevelManager::getEnemies() const
{
    return enemies;
}
std::vector<std::shared_ptr<Enemy>>& LevelManager::getEnemies()
{
    return enemies;
}

void LevelManager::trySpawnEnemies(int currentTurn, const Player& player)
{
    if (spawning)
    {
        spawning = false;
        spawnWaveEnemies(currentTurn, player);
        lastSpawnTurn = currentTurn;
        return;
    }

    bool shouldSpawn = false;

    if (enemies.empty())
    {
        shouldSpawn = true;
    }
    else if (currentLevel && static_cast<size_t>(currentWaveIndex) < currentLevel->waves.size())
    {
        const WaveInfo& nextWave = currentLevel->waves[currentWaveIndex];
        const int maxEnemies = static_cast<int>(currentLevel->fieldSize * 3 / 4);

        if (currentTurn >= nextWave.startTurn &&
            (enemies.size() + nextWave.enemies.size()) <= static_cast<size_t>(maxEnemies))
        {
            shouldSpawn = true;
        }
    }

    if(shouldSpawn)
    {
        spawning = true;
        std::cout<<"New wave is coming \n";
    }
}

void LevelManager::spawnWaveEnemies(int currentTurn, const Player& player)
{
    if (!currentLevel)
    {
        std::cerr << "Spawn error: currentLevel is nullptr\n";
        return;
    }

    if (currentWaveIndex >= currentLevel->waves.size())
    {

        return;
    }

    const WaveInfo& wave = currentLevel->waves[currentWaveIndex];
    std::cout << "\n[Wave " << currentWaveIndex + 1 << "] "
              << wave.name << " started\n";

    EnemyFactory factory;
    std::vector<int> freePositions = getFreePositions();
    std::random_device rd;
    std::mt19937 rng(rd());
    int spawnedCount = 0;

    for (std::vector<std::string>::const_iterator it = wave.enemies.begin();
         it != wave.enemies.end();
         ++it)
    {
        if (enemies.size() >= static_cast<size_t>(maxEnemies))
        {
            std::cout << "Enemy limit (" << maxEnemies << ") reached\n";
            break;
        }

        if (freePositions.empty())
        {
            std::cerr << "No free positions for " << *it << "\n";
            continue;
        }

        std::uniform_int_distribution<int> dist(0, freePositions.size() - 1);
        int randomIndex = dist(rng);
        int spawnPos = freePositions[randomIndex];

        occupiedCells[spawnPos] = true;

        std::shared_ptr<Enemy> enemy = factory.createEnemy(*it, currentLevel->fieldSize, *this, player.location);

        if (!enemy)
        {
            occupiedCells[spawnPos] = false;
            std::cerr << "Creation error " << *it << "\n";
            continue;
        }

        enemy->location = spawnPos;
        enemy->direction = (player.location > spawnPos);

        std::cout << "-> " << enemy->name << " created on position "
                 << enemy->location << ", direction: "
                 << (enemy->direction ? "right" : "left") << "\n";

        enemies.push_back(enemy);
        spawnedCount++;

        freePositions = getFreePositions();
    }

    currentWaveIndex++;
    std::cout << "Successfully created: " << spawnedCount << "/" << wave.enemies.size() << "\n";
    std::cout << "Total enemies: " << enemies.size() << "\n\n";
}

bool LevelManager::areAllEnemiesDefeated() const
{
    return enemies.empty();
}

const std::vector<bool>& LevelManager::getOccupiedCells() const
{
    return occupiedCells;
}

void LevelManager::updateOccupiedCells(const Player& player)
{
    std::fill(occupiedCells.begin(), occupiedCells.end(), false);
    occupiedCells[player.location] = true;
    for (const std::shared_ptr<Enemy>& enemy : enemies)
    {
        if (enemy && enemy->health > 0)
        {
            occupiedCells[enemy->location] = true;
        }
    }
}

std::vector<int> LevelManager::getFreePositions() const
{
    std::vector<int> freePositions;
    for (size_t i = 0; i < occupiedCells.size(); ++i)
    {
        if (!occupiedCells[i])
        {
            freePositions.push_back(i);
        }
    }

    return freePositions;
}
