#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "LevelData.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyFactory.h"
#include <vector>

class LevelManager
{
public:
    LevelManager(LevelData* levelData);
    ~LevelManager();

    void placePlayer(Player* player);
    void complete(Player* player);
    void spawnEnemies(int currentTurn, Player* player);
    bool areAllEnemiesDefeated() const;
    void checkAllCharacters();
    void removeDeadEnemies();

private:
    std::shared_ptr<Enemy> createEnemy(const std::string& enemyType, int location, bool direction);
    int findFreeSpawnLocation(Player* player);
    bool getDirectionTowardsPlayer(int enemyLocation, int playerLocation);

    LevelData* levelData;
    int currentWave;
    int lastSpawnTurn;
    std::vector<std::shared_ptr<Enemy>> enemies;

    EnemyFactory enemyFactory;
};

#endif