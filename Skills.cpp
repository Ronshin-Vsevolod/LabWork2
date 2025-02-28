#include "Skills.h"
#include "Personage.h"
#include <iostream>

Skill::Skill(const std::string& name, int cooldown, int coinCost, int skullCost)
    : name(name), cooldown(cooldown), cooldownTimer(0), preparing(false), coinCost(coinCost), skullCost(skullCost)
{
}

ClassicSkill::ClassicSkill(const std::string& name, int cooldown, int damage, const std::vector<int>& range_zones, int coinCost, int skullCost)
    : Skill(name, cooldown, coinCost, skullCost), damage(damage), range_zones(range_zones)
{
}

void ClassicSkill::applyEffect(Personage* user, const std::vector<Personage*>& targets)
{
    if (cooldownTimer > 0)
    {
        std::cout << "Навык " << name << " на перезарядке!\n";
        return;
    }

    std::cout << user->name << " использует " << name << "!\n";
    for (Personage* target : targets)
    {
        for (int zone : range_zones)
        {
            int targetLocation = user->location + zone;
            if (target->location == targetLocation)
            {
                target->health -= damage;
                std::cout << target->name << " получает " << damage << " урона!\n";
                if (target->health <= 0)
                {
                    std::cout << target->name << " повержен!\n";
                }
            }
        }
    }

    cooldownTimer = cooldown;
}

std::unordered_map<std::string, std::shared_ptr<Skill>> SkillRegistry =
{
    {
        "Fireball", std::make_shared<ClassicSkill>("Fireball", 5, 30, std::vector<int>{-1, 0, 1}, 10, 5)
    },
    {
        "Ice Blast", std::make_shared<ClassicSkill>("Ice Blast", 3, 20, std::vector<int>{-1, 0, 1}, 8, 3)
    },
    {
        "Lightning Strike", std::make_shared<ClassicSkill>("Lightning Strike", 7, 50, std::vector<int>{-2, -1, 0, 1, 2}, 15, 10)
    },
    {
        "Earthquake", std::make_shared<ClassicSkill>("Earthquake", 10, 70, std::vector<int>{-3, -2, -1, 0, 1, 2, 3}, 20, 15)
    }
};