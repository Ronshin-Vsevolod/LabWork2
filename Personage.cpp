#include "Personage.h"
#include <iostream>
#include <algorithm>

Personage::Personage(const std::string& name, int health, int damage, int location, bool direction)
    : name(name), health(health), damage(damage), location(location), direction(direction)
{
}

void Personage::prepareSkill(size_t skillIndex)
{
    if (skillIndex >= 0 && skillIndex < skills.size())
    {
        skills[skillIndex]->preparing = true;
        prepareStack.push_back(skills[skillIndex]);
        std::cout << name << " подготовил навык: " << skills[skillIndex]->name << "\n";
    }
    else
    {
        std::cout << name << " не удалось подготовить навык.\n";
    }
}

void Personage::useSkills(int fieldSize, std::vector<Personage*>& targets)
{
    for (std::shared_ptr<Skill> skill : prepareStack)
    {
        skill->preparing = false;
        std::cout << name << " использует навык: " << skill->name << "\n";
        skill->applyEffect(this, targets);
        checkAllCharacters(targets);
    }
    prepareStack.clear();
}

void Personage::moveRight(int fieldSize, const std::vector<Personage*>& enemies)
{
    int newLocation = location + 1;
    if (newLocation < fieldSize && !isLocationOccupied(newLocation, enemies))
    {
        location = newLocation;
        std::cout << name << " перемещён вправо. Новая позиция: " << location << "\n";
    }
    else
    {
        std::cout << name << " не может переместиться вправо.\n";
    }
}

void Personage::moveLeft(int fieldSize, const std::vector<Personage*>& enemies)
{
    int newLocation = location - 1;
    if (newLocation >= 0 && !isLocationOccupied(newLocation, enemies))
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