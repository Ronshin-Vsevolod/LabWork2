#ifndef PERSONAGE_H
#define PERSONAGE_H

#include <string>
#include <vector>
#include <memory>
#include "Skills.h"

class Personage
{
public:
    Personage(const std::string& name, int health, int damage, int location, bool direction);
    virtual ~Personage() = default;

    std::string name;
    int health;
    int damage;
    int location;
    bool direction;
    std::vector<std::shared_ptr<Skill>> skills;
    std::vector<std::shared_ptr<Skill>> prepareStack;

    virtual void prepareSkill(int skillIndex);
    virtual void useSkills(int fieldSize, const std::vector<Personage*>& enemies);
    virtual void moveRight(int fieldSize, const std::vector<Personage*>& enemies);
    virtual void moveLeft(int fieldSize, const std::vector<Personage*>& enemies);
    virtual void turnAround();
    virtual bool isLocationOccupied(int location, const std::vector<Personage*>& enemies);

protected:
    void checkAllCharacters(std::vector<Personage*>& enemies);
};

#endif