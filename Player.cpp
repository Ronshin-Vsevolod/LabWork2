#include "Player.h"
#include <iostream>

Player::Player(PlayerData* playerData)
    : Personage("Player", 100, 10, 0, true), playerData(playerData), swapCooldown(4), swapCooldownTimer(0)
{
    skills.push_back(SkillRegistry["Fireball"]);
    skills.push_back(SkillRegistry["Ice Blast"]);
}

void Player::swapWithEnemy(std::vector<Personage*>& enemies)
{
    if (swapCooldownTimer > 0)
    {
        std::cout << "Навык обмена местами на перезарядке!\n";
        return;
    }

    for (Personage* enemy : enemies)
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

    for (std::shared_ptr<Skill>& skill : skills)
    {
        if (!skill->preparing && skill->cooldownTimer > 0)
        {
            skill->cooldownTimer--;
        }
    }
}

void Player::swapPreparedSkills(int index1, int index2)
{
    if (index1 >= 0 && index1 < prepareStack.size() && index2 >= 0 && index2 < prepareStack.size())
    {
        std::swap(prepareStack[index1], prepareStack[index2]);
        std::cout << "Навыки " << prepareStack[index1]->name << " и " << prepareStack[index2]->name << " поменяны местами.\n";
    }
    else
    {
        std::cout << "Неверные индексы для обмена навыков.\n";
    }
}

void Player::removePreparedSkill(int index)
{
    if (index >= 0 && index < prepareStack.size())
    {
        std::cout << "Навык " << prepareStack[index]->name << " убран из стэка готовности.\n";
        prepareStack[index]->cooldownTimer = 0;
        prepareStack.erase(prepareStack.begin() + index);
    }
    else
    {
        std::cout << "Неверный индекс для удаления навыка.\n";
    }
}

void Player::resetOnDeath()
{
    health = 100;
    maxHP = 100;
    std::cout << "Игрок погиб. Параметры сброшены.\n";
}