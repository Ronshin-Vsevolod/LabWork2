#include "LevelManager.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>
#include <algorithm>
#include <random>

LevelManager::LevelManager(LevelData* levelData)
    : levelData(levelData), currentWave(0), lastSpawnTurn(0) {}

LevelManager::~LevelManager()
{
    for (auto enemy : enemies)
    {
        delete enemy;
    }
}

void LevelManager::placePlayer(Player* player)
{
    player->location = levelData->fieldSize / 2 + 1;
    std::cout << "Игрок размещён на позиции " << player->location << ".\n";
}

void LevelManager::complete(Player* player)
{
    player->playerData->money += levelData->rewardMoney;
    player->playerData->skulls += levelData->rewardSkulls;
    std::cout << "Уровень " << levelData->levelNumber << " завершён! Награды: " << levelData->rewardMoney << " монет, " << levelData->rewardSkulls << " черепов.\n";
}

void LevelManager::spawnEnemies(int currentTurn, Player* player)
{
    try
    {
        if (currentWave < levelData->waves.size())
        {
            auto wave = levelData->waves[currentWave];
            int minEnemies = wave.size() > 1 ? std::stoi(wave[1]) : 0;
            int turnInterval = wave.size() > 2 ? std::stoi(wave[2]) : 5;

            if (enemies.size() <= minEnemies && (enemies.empty() || currentTurn - lastSpawnTurn >= turnInterval))
            {
                for (size_t i = 3; i < wave.size(); ++i)
                {
                    int spawnLocation = findFreeSpawnLocation(player);
                    if (spawnLocation != -1)
                    {
                        bool direction = getDirectionTowardsPlayer(spawnLocation, player->location);
                        auto enemy = createEnemy(wave[i], spawnLocation, direction);
                        enemies.push_back(enemy);
                        std::cout << "Появился противник: " << enemy->name << " на позиции " << enemy->location << ".\n";
                    }
                }
                currentWave++;
                lastSpawnTurn = currentTurn;
            }
        }
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Ошибка при создании врага: " << e.what() << "\n";
    }
}

bool LevelManager::areAllEnemiesDefeated() const
{
    return enemies.empty();
}

void LevelManager::checkAllCharacters()
{
    for (auto& enemy : enemies)
    {
        if (enemy && enemy->health <= 0)
        {
            std::cout << enemy->name << " погиб!\n";
            delete enemy;
            enemy = nullptr;
        }
    }
}

void LevelManager::removeDeadEnemies()
{
    auto it = std::remove_if(enemies.begin(), enemies.end(), [](std::shared_ptr<Enemy> enemy)
    {
        return enemy == nullptr || enemy->health <= 0;
    });
    enemies.erase(it, enemies.end());
}

std::shared_ptr<Enemy> LevelManager::createEnemy(const std::string& enemyType, int location, bool direction)
{
    return enemyFactory.createEnemy(enemyType, location, direction);
}

int LevelManager::findFreeSpawnLocation(Player* player)
{
    std::vector<int> occupiedLocations;
    for (auto enemy : enemies)
    {
        if (enemy)
        {
            occupiedLocations.push_back(enemy->location);
        }
    }
    occupiedLocations.push_back(player->location);

    std::vector<int> freeLocations;
    for (int loc = 0; loc < levelData->fieldSize; ++loc)
    {
        if (std::find(occupiedLocations.begin(), occupiedLocations.end(), loc) == occupiedLocations.end() &&
            abs(loc - player->location) > 1)
        {
            freeLocations.push_back(loc);
        }
    }

    if (!freeLocations.empty())
    {
        return freeLocations[rand() % freeLocations.size()];
    }

    for (int loc = 0; loc < levelData->fieldSize; ++loc)
    {
        if (std::find(occupiedLocations.begin(), occupiedLocations.end(), loc) == occupiedLocations.end())
        {
            freeLocations.push_back(loc);
        }
    }

    if (!freeLocations.empty())
    {
        return freeLocations[rand() % freeLocations.size()];
    }

    return -1;
}

bool LevelManager::getDirectionTowardsPlayer(int enemyLocation, int playerLocation)
{
    return enemyLocation < playerLocation;
}