#ifndef ENEMY_H
#define ENEMY_H

#include "Personage.h"
#include "EnemyAI.h"

class Enemy : public Personage
{
public:
    Enemy(const std::string& name, int health, int damage, int location, bool direction, EnemyAI* ai = nullptr);
    virtual ~Enemy() = default;

    int path;
    EnemyAI* ai;

    virtual void makeTurn();
};

#endif