#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Personage;

class Skill
{
public:
    Skill(const std::string& name, int cooldown);
    virtual ~Skill() = default;

    std::string name;
    int cooldown;
    int cooldownTimer;
    bool preparing;
    int coinCost;
    int skullCost;

    virtual void applyEffect(Personage* user, const std::vector<Personage*>& targets) = 0;
};

class ClassicSkill : public Skill
{
public:
    ClassicSkill(const std::string& name, int cooldown, int damage, const std::vector<int>& range_zones);

    int damage;
    std::vector<int> range_zones;

    void applyEffect(Personage* user, const std::vector<Personage*>& targets) override;
};

extern std::unordered_map<std::string, std::shared_ptr<Skill>> SkillRegistry;

#endif