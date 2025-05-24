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
        std::cout << "The skill of swapping places on a reload!\n";
        return;
    }

    for (std::shared_ptr<Enemy>& enemy : enemies)
    {
        if (enemy && enemy->location == location + (direction ? 1 : -1))
        {
            std::swap(location, enemy->location);
            std::cout << name << " swapped places with " << enemy->name << "!\n";

            swapCooldownTimer = swapCooldown;
            return;
        }
    }

    std::cout << "No enemy nearby to swap places!\n";
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
        std::cout << "Skills " << prepareStack[index1]->name << " and " << prepareStack[index2]->name << " swapped.\n";
    }
    else
    {
        std::cout << "Invalid indices for swapping skills.\n";
    }
}

void Player::removePreparedSkill(size_t index)
{
    if (index < prepareStack.size())
    {
        std::cout << "Skill " << prepareStack[index]->name << " removed from the prepare stack.\n";
        prepareStack[index]->cooldownTimer = 0;
        prepareStack.erase(prepareStack.begin() + index);
    }
    else
    {
        std::cout << "Invalid index for removing skill.\n";
    }
}
