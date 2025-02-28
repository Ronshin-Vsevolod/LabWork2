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
    Skill(const std::string& name, int cooldown, int coinCost, int skullCost);
    virtual ~Skill() = default;

    std::string name;
    int cooldown;
    int cooldownTimer;
    bool preparing;
    int coinCost;
    int skullCost;

    virtual void applyEffect(Personage* user, const std::vector<Personage*>& targets) = 0;
    virtual void upgrade() = 0;
};

class ClassicSkill : public Skill
{
public:
    ClassicSkill(const std::string& name, int cooldown, int damage, const std::vector<int>& range_zones, int coinCost, int skullCost);

    int damage;
    std::vector<int> range_zones;

    void applyEffect(Personage* user, const std::vector<Personage*>& targets) override;
    void upgrade() override;
};

extern std::unordered_map<std::string, std::shared_ptr<Skill>> SkillRegistry;

#endif