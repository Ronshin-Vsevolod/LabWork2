/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "Effect.h"
#include "Personage.h"
#include <iostream>
#include <algorithm>
#include <functional>

std::vector<Personage*> SingleTargetAreaStrategy::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                                           const std::vector<int>& rangeZones) const
{
    std::vector<Personage*> targets;

    if (!user) return targets;

    for (Personage* target : allTargets)
    {
        if (!target) continue;

        for (int zone : rangeZones)
        {

            int effectiveZone = user->direction ? zone : -zone;
            int targetPos = user->location + effectiveZone;

            if (target->location == targetPos)
            {
                targets.push_back(target);
                return targets;
            }
        }
    }

    return targets;
}

std::vector<Personage*> AllTargetsAreaStrategy::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                                         const std::vector<int>& rangeZones) const
{
    std::vector<Personage*> targets;

    if (!user) return targets;

    std::vector<int> targetPositions;
    for (int zone : rangeZones)
    {
        int effectiveZone = user->direction ? zone : -zone;
        targetPositions.push_back(user->location + effectiveZone);
    }

    for (Personage* target : allTargets)
    {
        if (!target) continue;


        for (int targetPos : targetPositions)
        {
            if (target->location == targetPos)
            {
                targets.push_back(target);
                break;
            }
        }
    }

    return targets;
}



std::vector<Personage*> PlayerTargetAreaStrategy::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                                           const std::vector<int>& rangeZones) const
{
    std::vector<Personage*> targets;

    for (Personage* target : allTargets)
    {
        if (target && target->getName().find("Player") != std::string::npos)
        {
            targets.push_back(target);
                break;
        }
    }

    if (targets.empty() && user && user->getName().find("Player") != std::string::npos)
    {
        targets.push_back(user);
    }

    return targets;
}

std::vector<Personage*> EnemyTargetAreaStrategy::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                                          const std::vector<int>& rangeZones) const
{
    std::vector<Personage*> targets;

    for (Personage* target : allTargets)
    {
        if (target && target->getName().find("Player") == std::string::npos)
        {
            targets.push_back(target);
        }
    }

    return targets;
}

std::vector<Personage*> AllEnemiesAreaStrategy::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                                         const std::vector<int>& rangeZones) const
{
    std::vector<Personage*> targets;

    if (!user) return targets;

    for (Personage* target : allTargets)
    {
        if (target && target != user)
        {
            targets.push_back(target);
        }
    }

    return targets;
}

std::vector<Personage*> LowHealthTargetAreaStrategy::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                                              const std::vector<int>& rangeZones) const
{
    std::vector<Personage*> targets;

    for (Personage* target : allTargets)
    {
        if (target && target->health < target->maxHealth)
        {
            targets.push_back(target);
            break;
        }
    }

    return targets;
}



bool ReplaceConflictStrategy::resolveConflict(const std::shared_ptr<Effect>& existingEffect,
                                             const std::shared_ptr<Effect>& newEffect) const
{
    return true;
}

bool ExtendConflictStrategy::resolveConflict(const std::shared_ptr<Effect>& existingEffect,
                                            const std::shared_ptr<Effect>& newEffect) const
{
    if (existingEffect && newEffect)
    {
        existingEffect->duration += newEffect->duration;
        return false;
    }

    return true;
}

Effect::Effect(const std::string& name, int duration, std::shared_ptr<AreaStrategy> areaStrategy, std::shared_ptr<ConflictStrategy> conflictStrategy)
    : name(name), duration(duration),
      areaStrategy(areaStrategy),
      conflictStrategy(conflictStrategy)
      {}

void Effect::decreaseDuration()
{
    if (duration > 0)
    {
        duration--;
    }
}

std::vector<Personage*> Effect::getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                          const std::vector<int>& rangeZones) const
{
    return areaStrategy->getTargets(user, allTargets, rangeZones);
}

bool Effect::resolveConflict(const std::shared_ptr<Effect>& otherEffect) const
{
    if (!conflictStrategy)
    {
        return true;
    }

    std::shared_ptr<Effect> safeThis(const_cast<Effect*>(this), [](Effect*){});

    return conflictStrategy->resolveConflict(safeThis, otherEffect);
}

ShieldEffect::ShieldEffect()
        : Effect("Shield", 0,
        std::make_shared<SingleTargetAreaStrategy>(),
        std::make_shared<ReplaceConflictStrategy>())
{
    isFirstTrigger = true;
}

auto ShieldEffect::getTriggerType() const -> TriggerType
{
    return isFirstTrigger ? TriggerType::ON_DAMAGE : TriggerType::ON_AFTER_DAMAGE;
}

void ShieldEffect::apply(Personage* target)
{
    if (isFirstTrigger)
    {
        savedHealth = target->health;
        isFirstTrigger = false;
    }
    else
    {
        target->health = savedHealth;
    }
}

void ShieldEffect::remove(Personage* target){}

void HealingEffect::apply(Personage* target)
{
    if (target)
    {

        target->health += value;

        if (target->health > target->maxHealth)
        {
            target->health = target->maxHealth;
        }
    }
}

void DamageEffect::apply(Personage* target)
{
    if (target) target->takeDamage(value);
}

void DamageBoostEffect::apply(Personage* target)
{
    if (target) target->damage += value;
}

void DamageBoostEffect::remove(Personage* target)
{
    if (target) target->damage -= value;
}

