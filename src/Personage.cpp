/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "Personage.h"
#include "Skills.h"
#include "Effect.h"
#include <iostream>
#include <algorithm>

Personage::Personage(const std::string& name, int health, int maxHealth, int location, bool direction)
    : name(name), health(health), maxHealth(health), damage(0), location(location), direction(direction){}

void Personage::prepareSkill(size_t skillIndex)
{
    if (skillIndex >= 0 && skillIndex < skills.size())
    {
        if (skills[skillIndex]->cooldownTimer > 0)
        {
            std::cout << "Навык " << skills[skillIndex]->name
                     << " на перезарядке (осталось: " << skills[skillIndex]->cooldownTimer << " ходов)!\n";
            return;
        }

        for (const auto& skill : prepareStack)
        {
            if (skill == skills[skillIndex])
            {
                std::cout << "Навык " << skills[skillIndex]->name << " уже подготовлен!\n";
                return;
            }
        }

        skills[skillIndex]->preparing = true;
        prepareStack.push_back(skills[skillIndex]);
        std::cout << name << " подготовил навык: " << skills[skillIndex]->name << "\n";
    }
    else
    {
        std::cout << name << " не удалось подготовить навык.\n";
    }
}

void Personage::useSkills(int fieldSize, std::vector<Personage*>& enemies)
{
    for (auto& skill : prepareStack)
    {
        if (skill->cooldownTimer > 0)
        {
            std::cout << "Навык " << skill->name << " на перезарядке!\n";
            continue;
        }

        skill->useSkill(this, enemies);
        skill->preparing = false;
        skill->cooldownTimer = skill->cooldown;
    }
    prepareStack.clear();
}

void Personage::moveRight(int fieldSize, const std::vector<bool>& occupiedCells)
{
    int newLocation = location + 1;
    if (newLocation < fieldSize && !occupiedCells[newLocation])
    {
        location = newLocation;
        std::cout << name << " перемещён вправо. Новая позиция: " << location << "\n";
    }
    else
    {
        std::cout << name << " не может переместиться вправо.\n";
    }
}

void Personage::moveLeft(int fieldSize, const std::vector<bool>& occupiedCells)
{
    int newLocation = location - 1;
    if (newLocation >= 0 && !occupiedCells[newLocation])
    {
        location = newLocation;
        std::cout << name << " перемещён влево. Новая позиция: " << location << "\n";
    }
    else
    {
        std::cout << name << " не может переместиться влево.\n";
    }
}

void Personage::turnAround()
{
    direction = !direction;
    std::cout << name << " повёрнут в " << (direction ? "право" : "лево") << ".\n";
}

bool Personage::isLocationOccupied(int location, const std::vector<Personage*>& enemies)
{
    for (Personage* enemy : enemies)
    {
        if (enemy->location == location)
        {
            return true;
        }
    }
    return false;
}

void Personage::checkAllCharacters(std::vector<Personage*>& enemies)
{
    std::vector<Personage*>::iterator it = std::remove_if(enemies.begin(), enemies.end(), [](Personage* enemy)
    {
        return enemy->health <= 0;
    });
    enemies.erase(it, enemies.end());

    if (health <= 0)
    {
        std::cout << name << " погиб!\n";
    }
}

void Personage::applyEffects()
{
    for (const auto& effect : effects)
    {
        effect->apply(this);
    }
}

void Personage::addEffect(std::shared_ptr<Effect> effect)
{
    if (!effect)
    {
        std::cerr << "Попытка добавить nullptr эффект!" << std::endl;
        return;
    }

    bool shouldAdd = true;
    for (auto it = effects.begin(); it != effects.end();)
    {
        if (!(*it)->resolveConflict(effect))
        {
            it = effects.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (shouldAdd)
    {
        effects.push_back(effect);

        if (static_cast<int>(effect->getTriggerType()) == 0)
        {
            effect->apply(this);
        }
    }
}

void Personage::triggerEffect(int triggerType)
{
    for (auto it = effects.begin(); it != effects.end();)
    {
        if (static_cast<int>((*it)->getTriggerType()) == triggerType)
        {
            (*it)->apply(this);
            ++it;
        }
        else
        {
            ++it;
        }
    }
}

void Personage::decreaseEffectsDuration()
{
    for (auto it = effects.begin(); it != effects.end();)
    {
        (*it)->decreaseDuration();
        if ((*it)->duration <= 0)
        {
            (*it)->remove(this);
            it = effects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Personage::takeDamage(int damage)
{
    triggerEffect(4);

    health -= damage;
    std::cout << name << " получает " << damage << " урона! Оставшееся здоровье: " << health << "\n";
}

void Personage::dealDamage(Personage* target, int damage)
{
    if (target)
    {
        triggerEffect(6);
        target->takeDamage(damage);
    }
}

