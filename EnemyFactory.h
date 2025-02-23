#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include "Enemy.h"
#include <unordered_map>
#include <functional>
#include <string>

class EnemyFactory
{
public:
    EnemyFactory();

    std::shared_ptr<Enemy> createEnemy(const std::string& enemyType, int location, bool direction);

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<Enemy>(int, bool)>> enemyTypes;
};

#endif