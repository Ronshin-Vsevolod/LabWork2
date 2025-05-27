/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "EnemyAI.h"
#include "EnemyFactory.h"
#include "LevelManager.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

EnemyAI::EnemyAI(int fieldSize)
    : fieldSize(fieldSize){}

void EnemyAI::makeTurn(Enemy* enemy, Player& player, const LevelManager& levelManager)
{
    if (enemy->path == 0)
    {
        handlePathZero(enemy, player, levelManager);
    }
    else if (enemy->path > 0)
    {
        handlePositivePath(enemy);
    }
    else if (enemy->path == -1)
    {
        handleUseSkills(enemy, player, levelManager);
    }
    else if (enemy->path == -2)
    {
        handleTurnAround(enemy, player, levelManager);
    }
    else if (enemy->path == -3)
    {
        handleMoveRight(enemy, player, levelManager);
    }
    else if (enemy->path == -4)
    {
        handleMoveLeft(enemy, player, levelManager);
    }
}

void EnemyAI::handlePathZero(Enemy* enemy, const Player& player, const LevelManager& lm)
{

    if (tryTriggerPreparedSkills(enemy, player))
    {
        enemy->path = -1;
        return;
    }

    if (needsTurnToFacePlayer(enemy, player))
    {
        enemy->path = -2;
        return;
    }

    if (tryPrepareAvailableSkill(enemy, player))
    {

        return;
    }

    if (tryMoveTowardPlayer(enemy, lm))
    {
        return;
    }

    handleForcedInaction(enemy);
}

bool EnemyAI::tryTriggerPreparedSkills(Enemy* enemy, const Player& player)
{
    if (enemy->prepareStack.empty()) return false;

    const int distance = abs(enemy->location - player.location);
    for (const auto& skill : enemy->prepareStack)
    {
        if (isPlayerInSkillRange(skill.get(), distance))
        {
            std::cout << "[AI] " << enemy->name << " is going to use "
                     << skill->name << " (distance: " << distance << ")\n";
            return true;
        }
    }
    return false;
}

bool EnemyAI::needsTurnToFacePlayer(Enemy* enemy, const Player& player)
{
    const bool correctDirection = (player.location > enemy->location) ? enemy->direction : !enemy->direction;

    if (!correctDirection)
    {
        std::cout << "[AI] " << enemy->name << " should turn to the player\n";
        return true;
    }
    return false;
}

bool EnemyAI::tryPrepareAvailableSkill(Enemy* enemy, const Player& player)
{
    const int distance = abs(enemy->location - player.location);
    int availableSkillIndex = -1;

    for (size_t i = 0; i < enemy->skills.size(); ++i)
    {
        if (enemy->skills[i]->cooldownTimer > 0) continue;

        bool alreadyPrepared = std::any_of(
            enemy->prepareStack.begin(),
            enemy->prepareStack.end(),
            [&](const auto& skill)
            {
                return skill->name == enemy->skills[i]->name;
            }
        );

        if (alreadyPrepared) continue;

        if (availableSkillIndex == -1)
        {
            availableSkillIndex = i;
        }

        if (isPlayerInSkillRange(enemy->skills[i].get(), distance))
        {
            enemy->path = i + 1;
            std::cout << "[AI] " << enemy->name << " planned to use "
                     << enemy->skills[i]->name << " (can hit the player at a distance of "
                     << distance << ")\n";
            return true;
        }
    }


    if (availableSkillIndex != -1)
    {
        enemy->path = availableSkillIndex + 1;
        std::cout << "[AI] " << enemy->name << " planned to use "
                 << enemy->skills[availableSkillIndex]->name << " (can't hit the player now)\n";
        return true;
    }

    return false;
}


bool EnemyAI::tryMoveTowardPlayer(Enemy* enemy, const LevelManager& lm)
{
    const int step = enemy->direction ? 1 : -1;
    const int targetPos = enemy->location + step;

    if (targetPos < 0 || targetPos >= fieldSize)
    {
        std::cout << "[AI] " << enemy->name << " hit the edge of the field\n";
        return false;
    }

    if (!lm.getOccupiedCells()[targetPos])
    {
        enemy->path = (step > 0) ? -3 : -4;
        std::cout << "[AI] " << enemy->name << " moves to position "
                 << targetPos << "\n";
        return true;
    }

    std::cout << "[AI] " << enemy->name << " blocked on position "
             << enemy->location << "\n";
    return false;
}


void EnemyAI::handleForcedInaction(Enemy* enemy)
{
    std::cout << "[AI] " << enemy->name << " can't perform an action: "
             << "all options are exhausted\n";
    enemy->path = 0;
}

void EnemyAI::handlePositivePath(Enemy* enemy)
{
    int skillIndex = enemy->path - 1;
    if (skillIndex >= 0 && static_cast<size_t>(skillIndex) < enemy->skills.size())
    {
        enemy->prepareSkill(skillIndex);
        std::cout << enemy->name << " prepared "
                 << enemy->skills[skillIndex]->name << "\n";
    }
    enemy->path = 0;
}

void EnemyAI::handleUseSkills(Enemy* enemy, Player& player, const LevelManager& levelManager)
{
    if (!enemy->prepareStack.empty())
    {
        std::cout << enemy->name << " uses prepared skills!\n";
    }

    std::vector<Personage*> targets = {&player};
    enemy->useSkills(fieldSize, targets);

    resetAllCooldowns(enemy);
    enemy->path = 0;

    makeTurn(enemy, player, levelManager);
}

void EnemyAI::handleTurnAround(Enemy* enemy, Player& player, const LevelManager& levelManager)
{
    std::cout << enemy->name << " turns around!";
    enemy->turnAround();
    std::cout << " Now looks " << (enemy->direction ? "right" : "left") << "\n";
    enemy->path = 0;

    makeTurn(enemy, player, levelManager);
}

void EnemyAI::handleMoveRight(Enemy* enemy, Player& player, const LevelManager& levelManager)
{
    const std::vector<bool>& occupied = levelManager.getOccupiedCells();
    const int newPos = enemy->location + 1;

    if(newPos < fieldSize && !occupied[newPos])
    {
        enemy->location = newPos;
        std::cout << enemy->name << " moved right to position " << newPos << "\n";
    }
    else
    {
        std::cout << enemy->name << " can't move right (position " << newPos << " is occupied or outside the field)\n";
    }

    enemy->path = 0;

    makeTurn(enemy, player, levelManager);
}

void EnemyAI::handleMoveLeft(Enemy* enemy, Player& player, const LevelManager& levelManager)
{
    const std::vector<bool>& occupied = levelManager.getOccupiedCells();
    const int currentPosition = enemy->location;
    const int newPosition = currentPosition - 1;

    if (newPosition >= 0 && !occupied[newPosition])
    {
        enemy->location = newPosition;
        std::cout << enemy->name << " moved to the left. New position: " << newPosition << "\n";
    }
    else
    {
        std::cout << enemy->name << " can't move left (position " << newPosition << " is occupied or outside the field)\n";
    }

    enemy->path = 0;

    makeTurn(enemy, player, levelManager);
}

bool EnemyAI::isPlayerInSkillRange(const Skill* skill, int distance) const
{
    const std::vector<int>& zones = skill->getRangeZones();
    return std::find(zones.begin(), zones.end(), distance) != zones.end();
}

int EnemyAI::chooseSkillToPrepare(Enemy* enemy, const std::vector<std::shared_ptr<Skill>>& availableSkills, const Player& player)
{
    const int distance = abs(enemy->location - player.location);

    for (size_t i = 0; i < availableSkills.size(); ++i)
    {
        if (availableSkills[i]->cooldownTimer == 0 && isPlayerInSkillRange(availableSkills[i].get(), distance))
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void EnemyAI::resetAllCooldowns(Enemy* enemy)
{
    for (std::shared_ptr<Skill>& skill : enemy->skills)
    {
        skill->cooldownTimer = 0;
    }
}
