#include "Player.h"
#include <iostream>

Player::Player(PlayerData* playerData)
    : Personage("Player", 100, 10, 0, true), playerData(playerData), swapCooldown(4), swapCooldownTimer(0) {}

void Player::swapWithEnemy(std::vector<Personage*>& enemies)
{
    if (swapCooldownTimer > 0)
    {
        std::cout << "Навык обмена местами на перезарядке!\n";
        return;
    }

    for (auto& enemy : enemies)
    {
        if (enemy && enemy->location == location + (direction ? 1 : -1))
        {
            std::swap(location, enemy->location);
            std::cout << name << " поменялся местами с " << enemy->name << "!\n";

            swapCooldownTimer = swapCooldown;
            return;
        }
    }

    std::cout << "Рядом нет противника для обмена местами!\n";
}

void Player::updateCooldowns()
{
    if (swapCooldownTimer > 0)
    {
        swapCooldownTimer--;
    }

    for (auto& skill : skills)
    {
        if (!skill->preparing && skill->cooldownTimer > 0)
        {
            skill->cooldownTimer--;
        }
    }
}

void Player::resetOnDeath()
{
    health = 100;
    maxHP = 100;
    std::cout << "Игрок погиб. Параметры сброшены.\n";
}