/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <vector>
#include <memory>
#include <iostream>
#include "LevelData.h"

class Enemy;
class EnemyFactory;
class Player;

class LevelManager
{
private:
    const LevelData* currentLevel;
    std::vector<bool> occupiedCells;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::unique_ptr<EnemyFactory> enemyFactory;
    bool spawning;
    int lastSpawnTurn;
    size_t currentWaveIndex;
    int maxEnemies;
    void spawnWaveEnemies(int currentTurn, const Player& player);

public:
    LevelManager(const LevelData* levelData);
    ~LevelManager() = default;
    LevelData* getLevelData();

    void placePlayer(Player* player);
    void removeDeadEnemies(const Player& player);
    bool areAllEnemiesDefeated() const;
    const std::vector<bool>& getOccupiedCells() const;
    void updateOccupiedCells(const Player& player);
    std::vector<int> getFreePositions() const;
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const;
    std::vector<std::shared_ptr<Enemy>>& getEnemies();
    void trySpawnEnemies(int currentTurn, const Player& player);
};

#endif
