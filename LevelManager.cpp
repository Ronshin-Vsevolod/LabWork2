#include "LevelManager.h"

LevelManager::LevelManager(LevelData* levelData)
    : currentLevel(levelData)
{
}

LevelData* LevelManager::getLevelData()
{
    return currentLevel;
}

void LevelManager::placePlayer(Personage* player)
{
    player->location = currentLevel->fieldSize / 2;
}

void LevelManager::removeDeadEnemies()
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
}

std::vector<std::shared_ptr<Enemy>> LevelManager::getEnemies()
{
    return enemies;
}

void LevelManager::spawnEnemies(int turn, Personage* player)
{
    EnemyFactory factory;
    for (const auto& wave : currentLevel->waves)
    {
        if (turn >= std::stoi(wave[1]) && turn <= std::stoi(wave[2]))
        {
            for (size_t i = 3; i < wave.size(); ++i)
            {
                enemies.push_back(factory.createEnemy(wave[i], player->location + (i - 3), true));
            }
        }
    }
}

bool LevelManager::areAllEnemiesDefeated()
{
    return enemies.empty();
}