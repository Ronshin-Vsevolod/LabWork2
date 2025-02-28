#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "LevelData.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class LevelManager
{
public:
    LevelManager(LevelData* levelData);
    LevelData* getLevelData();
    void placePlayer(Personage* player);
    void removeDeadEnemies();
    std::vector<std::shared_ptr<Enemy>> getEnemies();
    void spawnEnemies(int turn, Personage* player);
    bool areAllEnemiesDefeated();

private:
    LevelData* currentLevel;
    std::vector<std::shared_ptr<Enemy>> enemies;
};

#endif