#include "EnemyAI.h"
#include "Enemy.h"
#include <algorithm>
#include <iostream>

EnemyAI::EnemyAI(Player* player, int fieldSize)
    : player(player), fieldSize(fieldSize) {}

void EnemyAI::makeTurn(Enemy* enemy)
{
    if (enemy->path == 0)
    {
        handlePathZero(enemy);
    }
    else if (enemy->path > 0)
    {
        handlePositivePath(enemy);
    }
    else if (enemy->path == -1)
    {
        handleUseSkills(enemy);
    }
    else if (enemy->path == -2)
    {
        handleTurnAround(enemy);
    }
    else if (enemy->path == -3)
    {
        handleMoveRight(enemy);
    }
    else if (enemy->path == -4)
    {
        handleMoveLeft(enemy);
    }
}

void EnemyAI::handlePathZero(Enemy* enemy)
{
    if (enemy->prepareStack.size() >= 4)
    {
        std::cout << enemy->name << " не может подготовить больше навыков. Стек переполнен!\n";
        return;
    }

    std::vector<std::shared_ptr<Skill>> availableSkills;
    for (const std::shared_ptr<Skill>& skill : enemy->skills)
    {
        if (!skill->preparing)
        {
            availableSkills.push_back(skill);
        }
    }

    if (isPlayerInSkillRange(enemy, availableSkills))
    {
        int skillIndex = chooseSkillToPrepare(enemy, availableSkills);
        if (skillIndex != -1)
        {
            std::cout << enemy->name << " готовится подготовить навык!\n";
            enemy->prepareSkill(skillIndex);
            return;
        }
    }

    if ((enemy->direction && player->location < enemy->location) ||
        (!enemy->direction && player->location > enemy->location))
    {
        std::cout << enemy->name << " готовится развернуться!\n";
        enemy->path = -2;
        return;
    }

    if (enemy->prepareStack.size() < 4)
    {
        int skillIndex = chooseSkillToPrepare(enemy, availableSkills);
        if (skillIndex != -1)
        {
            std::cout << enemy->name << " готовится подготовить навык!\n";
            enemy->prepareSkill(skillIndex);
            enemy->path = -2;
            return;
        }
    }

    if (player->location > enemy->location)
    {
        std::cout << enemy->name << " готовится переместиться вправо!\n";
        enemy->path = -3;
    }
    else
    {
        std::cout << enemy->name << " готовится переместиться влево!\n";
        enemy->path = -4;
    }
}

void EnemyAI::handlePositivePath(Enemy* enemy)
{
    int skillIndex = enemy->path - 1;
    if (skillIndex >= 0 && static_cast<size_t>(skillIndex) < enemy->skills.size())
    {
        enemy->prepareSkill(skillIndex);
    }
    enemy->path = 0;
}

void EnemyAI::handleUseSkills(Enemy* enemy)
{
    for (std::shared_ptr<Skill>& skill : enemy->prepareStack)
    {
        skill->applyEffect(enemy, {player});
        if (skill->cooldownTimer == 0)
        {
            skill->cooldownTimer = skill->cooldown;
        }
    }
    enemy->prepareStack.clear();
    enemy->path = 0;
}

void EnemyAI::handleTurnAround(Enemy* enemy)
{
    enemy->turnAround();
    enemy->path = 0;
}

void EnemyAI::handleMoveRight(Enemy* enemy)
{
    enemy->moveRight(fieldSize, {player});
    enemy->path = 0;
}

void EnemyAI::handleMoveLeft(Enemy* enemy)
{
    enemy->moveLeft(fieldSize, {player});
    enemy->path = 0;
}

bool EnemyAI::isPlayerInSkillRange(Enemy* enemy, const std::vector<std::shared_ptr<Skill>>& skills)
{
    for (const std::shared_ptr<Skill>& skill : skills)
    {
        if (std::shared_ptr<ClassicSkill> classicSkill = std::dynamic_pointer_cast<ClassicSkill>(skill))
        {
            return true;
        }
    }
    return false;
}

int EnemyAI::chooseSkillToPrepare(Enemy* enemy, const std::vector<std::shared_ptr<Skill>>& availableSkills)
{
    for (size_t i = 0; i < availableSkills.size(); ++i)
    {
        if (std::shared_ptr<ClassicSkill> classicSkill = std::dynamic_pointer_cast<ClassicSkill>(availableSkills[i]))
        {
            return i;
        }
    }
    return -1;
}