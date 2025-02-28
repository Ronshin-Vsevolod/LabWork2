#ifndef ENEMY_H
#define ENEMY_H

#include "Personage.h"
#include "EnemyAI.h"
#include <unordered_map>
#include <functional>
#include <string>

class EnemyAI;

class Enemy : public Personage
{
public:
    Enemy(const std::string& name, int health, int damage, int location, bool direction, EnemyAI* ai = nullptr);
    virtual ~Enemy() = default;

    int path;
    EnemyAI* ai;

    virtual void makeTurn();
};

class EnemyFactory
{
public:
    EnemyFactory();

    std::shared_ptr<Enemy> createEnemy(const std::string& enemyType, int location, bool direction);

private:
    std::unordered_map<std::string, std::function<std::shared_ptr<Enemy>(int, bool)>> enemyTypes;
};

class Goblin : public Enemy
{
public:
    Goblin(int location, bool direction, EnemyAI* ai = nullptr);
};

class Orc : public Enemy
{
public:
    Orc(int location, bool direction, EnemyAI* ai = nullptr);
};

#endif