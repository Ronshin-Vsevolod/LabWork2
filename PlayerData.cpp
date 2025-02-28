#include "PlayerData.h"
#include <iostream>

PlayerData::PlayerData()
    : money(0), skulls(0), health(100), maxHP(100)
{
}

void PlayerData::resetOnDeath()
{
    money = 0;
    health = 100;
    maxHP = 100;
    skills.clear();
    std::cout << "Игрок погиб. Данные сброшены.\n";
}