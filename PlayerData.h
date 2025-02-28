#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "Skills.h"
#include <vector>
#include <memory>

class PlayerData
{
public:
    PlayerData();
    void resetOnDeath();

    int money;
    int skulls;
    int health;
    int maxHP;
    std::vector<std::shared_ptr<Skill>> skills;
};

#endif