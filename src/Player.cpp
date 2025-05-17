/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "Player.h"
#include <iostream>

Player::Player(PlayerData* playerData)
    : Personage("Player", 6, 6, 0, true), playerData(playerData), swapCooldown(4), swapCooldownTimer(0){}

void Player::swapWithEnemy(std::vector<std::shared_ptr<Enemy>>& enemies)
{
    if (swapCooldownTimer > 0)
    {
        std::cout << "Навык обмена местами на перезарядке!\n";
        return;
    }

    for (std::shared_ptr<Enemy>& enemy : enemies)
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

void Player::swapPreparedSkills(size_t index1, size_t index2)
{
    if (index1 < prepareStack.size() && index2 < prepareStack.size())
    {
        std::swap(prepareStack[index1], prepareStack[index2]);
        std::cout << "Навыки " << prepareStack[index1]->name << " и " << prepareStack[index2]->name << " поменяны местами.\n";
    }
    else
    {
        std::cout << "Неверные индексы для обмена навыков.\n";
    }
}

void Player::removePreparedSkill(size_t index)
{
    if (index < prepareStack.size())
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
