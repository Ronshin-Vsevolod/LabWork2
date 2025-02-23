#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& name, int health, int damage, int location, bool direction, EnemyAI* ai)
    : Personage(name, health, damage, location, direction), path(0), ai(ai) {}

void Enemy::makeTurn()
{
    if (ai)
    {
        ai->makeTurn(this);
    }
}