/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "EnemyFactory.h"
#include "LevelManager.h"
#include "EnemyAI.h"
#include "Skills.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>

Enemy::Enemy(const std::string& name, int health, int maxHealth, int location, bool direction)
    : Personage(name, health, maxHealth, location, direction){}

void Enemy::setAI(std::unique_ptr<EnemyAI> ai)
{
    this->ai = std::move(ai);
}

void Enemy::makeTurn(Player& player, LevelManager& levelManager)
{
    if (!ai)
    {
        std::cerr << "AI не инициализирован для " << name << std::endl;
        return;
    }

    ai->makeTurn(this, player, levelManager);
}

EnemyFactory::EnemyFactory()
    : rng(std::random_device{}())
{
    creators["Ashigaru"] = [this](int loc, bool dir)
    {
        return std::make_shared<Ashigaru>(loc, dir);
    };
    creators["Twin Tachi"] = [this](int loc, bool dir)
    {
        return std::make_shared<TwinTachi>(loc, dir);
    };
    creators["Yari Master"] = [this](int loc, bool dir)
    {
        return std::make_shared<YariMaster>(loc, dir);
    };
    creators["Tetsudo Bandit"] = [this](int loc, bool dir)
    {
        return std::make_shared<TetsudobBandit>(loc, dir);
    };
    creators["Armored Giant"] = [this](int loc, bool dir)
    {
        return std::make_shared<ArmoredGiant>(loc, dir);
    };
    creators["Shaman"] = [this](int loc, bool dir)
    {
        return std::make_shared<Shaman>(loc, dir);
    };
    /*
    creators["Kamikaze"] = [this](int loc, bool dir)
    {
        return std::make_shared<Kamikaze>(loc, dir);
    };
    */
}

template<typename T>
std::shared_ptr<Enemy> EnemyFactory::createEnemy(int fieldSize, const LevelManager& lm, int playerLoc)
{
    std::vector<int> freePositions = lm.getFreePositions();
    if (freePositions.empty())
    {
        std::cerr << "[ОШИБКА] Нет свободных позиций для спавна\n";
        return nullptr;
    }

    std::uniform_int_distribution<int> posDist(0, freePositions.size() - 1);
    int spawnLocation = freePositions[posDist(rng)];
    bool spawnDirection = (playerLoc > spawnLocation);

    return std::make_shared<T>(spawnLocation, spawnDirection);
}

std::shared_ptr<Enemy> EnemyFactory::createEnemy(
    const std::string& type,
    int fieldSize,
    const LevelManager& lm,
    int playerLoc)
{
    std::unordered_map<std::string, std::function<std::shared_ptr<Enemy>(int, bool)>>::iterator it = creators.find(type);

    if (it == creators.end())
    {
        std::cerr << "ERROR: Unknown enemy type '" << type << "'\n";
        return nullptr;
    }

    std::vector<int> free = lm.getFreePositions();
    if (free.empty())
    {
        std::cerr << "ERROR: No free positions for enemy spawn\n";
        return nullptr;
    }

    bool dir = playerLoc > 0;

    std::shared_ptr<Enemy> enemy = (it->second)(0, dir);

    if (!enemy)
    {
        std::cerr << "ERROR: Failed to create enemy of type '" << type << "'\n";
        return nullptr;
    }

    if (enemy->name.empty() || enemy->health <= 0)
    {
        std::cerr << "WARNING: Invalid enemy properties. Name: '"
                 << enemy->name << "', Health: " << enemy->health << "\n";
    }

    enemy->setAI(std::make_unique<EnemyAI>(fieldSize));

    return enemy;
}

template std::shared_ptr<Enemy> EnemyFactory::createEnemy<Ashigaru>(int, const LevelManager&, int);
template std::shared_ptr<Enemy> EnemyFactory::createEnemy<TwinTachi>(int, const LevelManager&, int);
template std::shared_ptr<Enemy> EnemyFactory::createEnemy<YariMaster>(int, const LevelManager&, int);
template std::shared_ptr<Enemy> EnemyFactory::createEnemy<TetsudobBandit>(int, const LevelManager&, int);
template std::shared_ptr<Enemy> EnemyFactory::createEnemy<ArmoredGiant>(int, const LevelManager&, int);
template std::shared_ptr<Enemy> EnemyFactory::createEnemy<Shaman>(int, const LevelManager&, int);

int EnemyFactory::getRandomFreeLocation(int fieldSize, const std::vector<bool>& occupiedLocations)
{
    std::uniform_int_distribution<int> dist(0, fieldSize - 1);
    int location;
    do
    {
        location = dist(rng);
    }
    while (occupiedLocations[location]);
    return location;
}
/*

void Kamikaze::explode(Player& player, std::vector<std::shared_ptr<Enemy>>& enemies)
{
    std::cout << "Камикадзе взрывается при смерти!\n";


    if (abs(player.location - location) <= 1)
    {
        std::cout << "Игрок " << player.name << " получает 2 урона от взрыва Камикадзе!\n";
        player.health -= 2;
    }


    for (auto& enemy : enemies)
    {
        if (enemy.get() != this && abs(enemy->location - location) <= 1 && enemy->health > 0)
        {
            std::cout << "Противник " << enemy->name << " получает 2 урона от взрыва Камикадзе!\n";
            enemy->health -= 2;
        }
    }
}
*/
