#include "Enemy.h"
#include <iostream>

Enemy::Enemy(const std::string& name, int health, int damage, int location, bool direction, EnemyAI* ai)
    : Personage(name, health, damage, location, direction), path(0), ai(ai)
{
}

void Enemy::makeTurn()
{
    if (ai)
    {
        ai->makeTurn(this);
    }
}

EnemyFactory::EnemyFactory()
{
    enemyTypes["Goblin"] = [](int location, bool direction)
    {
        return std::make_shared<Goblin>(location, direction);
    };
    enemyTypes["Orc"] = [](int location, bool direction)
    {
        return std::make_shared<Orc>(location, direction);
    };
}

std::shared_ptr<Enemy> EnemyFactory::createEnemy(const std::string& enemyType, int location, bool direction)
{
    std::unordered_map<std::string, std::function<std::shared_ptr<Enemy>(int, bool)>>::iterator it = enemyTypes.find(enemyType);
    if (it != enemyTypes.end())
    {
        return it->second(location, direction);
    }
    throw std::invalid_argument("Неизвестный тип противника: " + enemyType);
}

Goblin::Goblin(int location, bool direction, EnemyAI* ai)
    : Enemy("Goblin", 30, 5, location, direction, ai)
{
    std::unordered_map<std::string, std::shared_ptr<Skill>>::iterator fireballIt = SkillRegistry.find("Fireball");
    if (fireballIt != SkillRegistry.end())
    {
        skills.push_back(fireballIt->second);
    }
}

Orc::Orc(int location, bool direction, EnemyAI* ai)
    : Enemy("Orc", 50, 10, location, direction, ai)
{
    std::unordered_map<std::string, std::shared_ptr<Skill>>::iterator iceBlastIt = SkillRegistry.find("Ice Blast");
    if (iceBlastIt != SkillRegistry.end())
    {
        skills.push_back(iceBlastIt->second);
    }
}