#ifndef ENEMY_H
#define ENEMY_H

#include "Personage.h"
#include "EnemyAI.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

class EnemyAI;

class Enemy : public Personage
{
public:
    Enemy(const std::string& name, int health, int damage, int location, bool direction, std::unique_ptr<EnemyAI> ai = nullptr);
    virtual ~Enemy() = default;
    
    int path = 0;
    
    void setAI(std::unique_ptr<EnemyAI> ai);
    void makeTurn();

    EnemyAI* getAI() const
    {
        return ai.get();
    }

protected:
    std::unique_ptr<EnemyAI> ai;
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
    Goblin(int location, bool direction, std::unique_ptr<EnemyAI> ai = nullptr);
    friend class EnemyAI;
};

class Orc : public Enemy
{
public:
    Orc(int location, bool direction, std::unique_ptr<EnemyAI> ai = nullptr);
    friend class EnemyAI;
};
#endif