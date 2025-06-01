/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <iostream>

class Personage;
class Effect;

#include "Personage.h"
#include "Effect.h"

class Skill
{
public:
    Skill(const std::string& name, int cooldown, int coinCost, int skullCost)
        : name(name), cooldown(cooldown), cooldownTimer(0), preparing(false), coinCost(coinCost), skullCost(skullCost){}
    virtual ~Skill() = default;

    std::string name;
    int cooldown;
    int cooldownTimer;
    bool preparing;
    int coinCost;
    int skullCost;

    virtual const std::vector<int>& getRangeZones() const = 0;
    virtual void useSkill(Personage* user, const std::vector<Personage*>& targets) = 0;
    virtual void upgrade() = 0;
};

class EffectSkill : public Skill
{
protected:
    std::vector<std::shared_ptr<Effect>> effectTemplates;
    std::vector<int> range_zones;
    std::unordered_map<std::string, int> effectValues;

public:
    EffectSkill(const std::string& name, int cooldown, const std::vector<int>& range_zones,
                int coinCost, int skullCost)
        : Skill(name, cooldown, coinCost, skullCost), range_zones(range_zones){}

    void addEffectTemplate(std::shared_ptr<Effect> effect, int value = 1)
    {
        effectTemplates.push_back(effect);
        effectValues[effect->name] = value;
    }

    void useSkill(Personage* user, const std::vector<Personage*>& targets) override
    {
        if (cooldownTimer > 0)
        {
            return;
        }

        cooldownTimer = cooldown;
    }

    const std::vector<int>& getRangeZones() const override {return range_zones;}

    void upgrade() override
    {
        if (cooldown > 1)
        {
            cooldown -= 1;
        }

        for (auto& [name, value] : effectValues)
        {
            value += 1;
        }
    }
};


class StandardEffectSkill : public EffectSkill
{
public:
    StandardEffectSkill(const std::string& name, int cooldown, const std::vector<int>& range_zones,
                       int coinCost, int skullCost)
        : EffectSkill(name, cooldown, range_zones, coinCost, skullCost){}

    void useSkill(Personage* user, const std::vector<Personage*>& targets) override
    {
        if (cooldownTimer > 0)
        {
            return;
        }

        cooldownTimer = cooldown;
    }
};

class ClassicSkill : public Skill
{
public:
    ClassicSkill(const std::string& name, int cooldown, int damage, const std::vector<int>& range_zones, int coinCost, int skullCost)
    : Skill(name, cooldown, coinCost, skullCost), damage(damage), range_zones(range_zones){}

    int damage;
    std::vector<int> range_zones;

    const std::vector<int>& getRangeZones() const override {return range_zones;}

    void useSkill(Personage* user, const std::vector<Personage*>& targets) override
    {
        if (cooldownTimer > 0)
        {
            std::cout << "Skill " << name << " on cooldown!\n";
            return;
        }

        std::cout << user->name << " uses " << name << "!\n";
        for (Personage* target : targets)
        {
            for (int zone : range_zones)
            {
                int targetLocation = user->location + zone;
                if (target->location == targetLocation)
                {
                    user->dealDamage(target, damage);
                    std::cout << target->name << " gets " << damage << " damage!\n";
                    if (target->health <= 0)
                    {
                        std::cout << target->name << " defeated!\n";
                    }
                }
            }
        }

        cooldownTimer = cooldown;
    }

    void upgrade() override
    {
        damage += 1;
        std::cout << "Skill " << name << " upgraded! New damage: " << damage << "\n";
    }
};


class HealingTotemSkill : public EffectSkill
{
public:
    HealingTotemSkill();
    void useSkill(Personage* user, const std::vector<Personage*>& targets) override;
};

class CurseSkill : public EffectSkill
{
public:
    CurseSkill();
};

class AncestralSpiritSkill : public EffectSkill
{
public:
    AncestralSpiritSkill();
};

extern std::unordered_map<std::string, std::shared_ptr<Skill>> SkillRegistry;

class SkillFactory
{
public:
    using Creator = std::function<std::shared_ptr<Skill>()>;
    static std::unordered_map<std::string, Creator> registry;

    template<typename T>
    static void registerSkill(const std::string& name)
    {
        if (std::is_same<T, ClassicSkill>::value)
        {
            registry[name] = []
            {
                return std::make_shared<ClassicSkill>("Classic Skill", 1, 1, std::vector<int>{1}, 0, 0);
            };
        }
        else if (std::is_same<T, StandardEffectSkill>::value)
        {
            registry[name] = []
            {
                return std::make_shared<StandardEffectSkill>("Standard Effect Skill", 1, std::vector<int>{1}, 0, 0);
            };
        }
    }

    static bool exists(const std::string& name)
    {
        return registry.find(name) != registry.end();
    }

    static std::shared_ptr<Skill> create(const std::string& name)
    {
        return registry.at(name)();
    }
};


inline void initializeSkills()
{
    SkillFactory::registerSkill<ClassicSkill>("Classic Skill");
    SkillFactory::registerSkill<StandardEffectSkill>("Standard Effect Skill");

    SkillFactory::registerSkill<HealingTotemSkill>("Healing Totem");
    SkillFactory::registerSkill<CurseSkill>("Curse");
    SkillFactory::registerSkill<AncestralSpiritSkill>("Ancestral Spirit");
}

#endif
