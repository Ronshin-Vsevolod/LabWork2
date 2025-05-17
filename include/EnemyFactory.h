/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef ENEMY_FACTORY_H
#define ENEMY_FACTORY_H

#include <memory>
#include <string>
#include <random>
#include <unordered_map>
#include <vector>
#include <functional>
#include "EnemyAI.h"
#include "Player.h"
#include "Personage.h"
#include "Skills.h"

class Enemy;
class EnemyAI;
class LevelManager;
class LevelData;

class Enemy : public Personage
{
public:
    Enemy(const std::string& name, int health, int maxHealth, int location, bool direction);
    virtual ~Enemy() = default;

    void setAI(std::unique_ptr<EnemyAI> ai);
    void makeTurn(Player& player, LevelManager& levelManager);

    int path = 0;

protected:
    std::unique_ptr<EnemyAI> ai;
};

class EnemyFactory
{
public:
    EnemyFactory();
    std::shared_ptr<Enemy> createEnemy(const std::string& type, int fieldSize, const LevelManager& lm, int playerLoc);

    template<typename T>
    std::shared_ptr<Enemy> createEnemy(int fieldSize, const LevelManager& lm,int playerLoc);

private:
    std::mt19937 rng;
    std::unordered_map<std::string, std::function<std::shared_ptr<Enemy>(int, bool)>> creators;
    int getRandomFreeLocation(int fieldSize, const std::vector<bool>& occupiedLocations);
};

class Ashigaru : public Enemy
{
public:
    Ashigaru(int location, bool direction)
        : Enemy("Ashigaru", 3, 3, location, direction)
    {

        skills.push_back(SkillFactory::create("Katana"));
    }
};

class TwinTachi : public Enemy
{
public:
    TwinTachi(int location, bool direction)
        : Enemy("Twin Tachi", 2, 2, location, direction)
    {

        skills.push_back(SkillFactory::create("Two katanas"));
    }
};

class YariMaster : public Enemy
{
public:
    YariMaster(int location, bool direction)
        : Enemy("Yari Master", 4, 4, location, direction)
    {

        skills.push_back(SkillFactory::create("Yari"));
    }
};

class TetsudobBandit : public Enemy
{
public:
    TetsudobBandit(int location, bool direction)
        : Enemy("Tetsudo Bandit", 4, 4, location, direction)
    {

        skills.push_back(SkillFactory::create("Tetsubo"));
    }
};

class ArmoredGiant : public Enemy
{
public:
    ArmoredGiant(int location, bool direction)
        : Enemy("Armored Giant", 8, 8, location, direction)
    {
        skills.push_back(SkillFactory::create("Ono"));
    }
};

class Shaman : public Enemy
{
public:
    Shaman(int location, bool direction)
        : Enemy("Shaman", 3, 3, location, direction)
    {
        skills.push_back(SkillFactory::create("Shaman"));
    }
};

#endif

