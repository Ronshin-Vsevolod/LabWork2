/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "Skills.h"
#include "Personage.h"
#include "Effect.h"
#include <iostream>
#include <limits>

std::unordered_map<std::string, SkillFactory::Creator> SkillFactory::registry =
{
    {
        "Two katanas",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Two katanas", 3, 2, std::vector{1, -1}, 5, 3);
        }
    },

    {
        "Katana",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Katana", 2, 2, std::vector{1}, 5, 3);
        }
    },

    {
        "Tetsubo",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Tetsubo", 6, 4, std::vector{1}, 10, 8);
        }
    },

    {
        "Odachi",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Odachi", 4, 3, std::vector{1, 2}, 8, 6);
        }
    },

    {
        "Yari",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Yari", 2, 2, std::vector{2}, 6, 4);
        }
    },

    {
        "Naginata",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Naginata", 3, 3, std::vector{2}, 8, 6);
        }
    },

    {
        "Kanabo",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Kanabo", 6, 5, std::vector{1}, 12, 10);
        }
    },

    {
        "Ono",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Ono", 5, 4, std::vector{1}, 10, 8);
        }
    },

    {
        "Tanto",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Tanto", 1, 1, std::vector{-1}, 3, 2);
        }
    },

    {
        "Holy Grenade",
        []() -> std::shared_ptr<Skill>
        {
            return std::make_shared<ClassicSkill>
            ("Holy Grenade", 1, 999, std::vector{-5, -4, -3, -2, -1, 1, 2, 3, 4, 5}, 0, 0);
        }
    },
};


HealingTotemSkill::HealingTotemSkill()
    : EffectSkill("Целебный тотем", 3, {0, 1, -1}, 2, 0) 
    {
    auto healingEffect = std::make_shared<HealingEffect>(
        1, 1,
        std::make_shared<LowHealthTargetAreaStrategy>(),
        std::make_shared<ExtendConflictStrategy>()
    );
    addEffectTemplate(healingEffect, 1);
    }

void HealingTotemSkill::useSkill(Personage* user, const std::vector<Personage*>& targets)
{
    if (cooldownTimer > 0)
    {
        std::cout << "Навык " << name << " на перезарядке!\n";
        return;
    }

    std::cout << user->name << " использует " << name << "!\n";

    auto healingEffect = std::make_shared<HealingEffect>(
        1, 1,
        std::make_shared<LowHealthTargetAreaStrategy>(),
        std::make_shared<ExtendConflictStrategy>()
    );


    std::vector<Personage*> effectTargets = healingEffect->getTargets(user, targets, range_zones);

    for (Personage* target : effectTargets)
    {
        if (target)
        {
            std::cout << "Эффект исцеления применен к " << target->name << "!\n";
            target->addEffect(healingEffect);
        }
    }

    cooldownTimer = cooldown;
}

CurseSkill::CurseSkill()
    : EffectSkill("Проклятие", 4, {1, 2}, 3, 1) 
    {
    auto damageReductionEffect = std::make_shared<DamageBoostEffect>(
        -1, 3,
        std::make_shared<PlayerTargetAreaStrategy>(),
        std::make_shared<ReplaceConflictStrategy>()
    );
    addEffectTemplate(damageReductionEffect, -1);
    }

AncestralSpiritSkill::AncestralSpiritSkill()
    : EffectSkill("Дух предков", 5, {0, 1}, 4, 2) 
    {
    auto damageBoostEffect = std::make_shared<DamageBoostEffect>(
        2, 3,
        std::make_shared<EnemyTargetAreaStrategy>(),
        std::make_shared<ExtendConflictStrategy>()
    );
    addEffectTemplate(damageBoostEffect, 2);
    }

