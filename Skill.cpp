#include "Skill.h"
#include <iostream>

Skill::Skill(const std::string& name, int cooldown)
    : name(name), cooldown(cooldown), cooldownTimer(0), preparing(false) {}

ClassicSkill::ClassicSkill(const std::string& name, int cooldown, int damage, const std::vector<int>& range_zones)
    : Skill(name, cooldown), damage(damage), range_zones(range_zones) {}

void ClassicSkill::applyEffect(Personage* user, const std::vector<Personage*>& targets)
{
    if (cooldownTimer > 0)
    {
        std::cout << "Навык " << name << " на перезарядке!\n";
        return;
    }

    std::cout << user->name << " использует " << name << "!\n";
    for (auto target : targets)
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
}