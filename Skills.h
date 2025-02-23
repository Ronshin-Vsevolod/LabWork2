#ifndef SKILLS_H
#define SKILLS_H

#include "Skill.h"
#include <unordered_map>
#include <memory>

class Fireball : public Skill
{
public:
    Fireball() : Skill("Fireball", 5) {}
    void applyEffect(Personage* user, const std::vector<Personage*>& targets) override
    {
        
    }
};

class IceBlast : public Skill
{
public:
    IceBlast() : Skill("Ice Blast", 3) {}
    void applyEffect(Personage* user, const std::vector<Personage*>& targets) override
    {

    }
};

struct SkillInfo
{
    std::shared_ptr<Skill> skill;
    int skullCost;
    int coinCost;
};

std::unordered_map<std::string, SkillInfo> SkillRegistry =
{
    {"Fireball", {std::make_shared<Fireball>(), 10, 20}},
    {"Ice Blast", {std::make_shared<IceBlast>(), 15, 30}},
};

bool upgradeSkill(std::shared_ptr<Skill> skill, int& coins)
{
    auto it = SkillRegistry.find(skill->name);
    if (it != SkillRegistry.end() && coins >= it->second.coinCost)
    {
        coins -= it->second.coinCost;
        skill->damage += 5;
        return true;
    }
    return false;
}

#endif