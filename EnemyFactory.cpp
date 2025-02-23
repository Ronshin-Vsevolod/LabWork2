#include "EnemyFactory.h"
#include "Goblin.h"
#include "Orc.h"

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
    auto it = enemyTypes.find(enemyType);
    if (it != enemyTypes.end())
    {
        return it->second(location, direction);
    }
    throw std::invalid_argument("Неизвестный тип противника: " + enemyType);
}