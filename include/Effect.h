/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <iostream>
#include "Personage.h"

class Personage;
class Effect;

class AreaStrategy
{
public:
    virtual ~AreaStrategy() = default;
    virtual std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                              const std::vector<int>& rangeZones) const = 0;
};

class SingleTargetAreaStrategy : public AreaStrategy
{
public:
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const override;
};

class AllTargetsAreaStrategy : public AreaStrategy
{
public:
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const override;
};

class PlayerTargetAreaStrategy : public AreaStrategy
{
public:
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const override;
};

class EnemyTargetAreaStrategy : public AreaStrategy
{
public:
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const override;
};

class AllEnemiesAreaStrategy : public AreaStrategy
{
public:
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const override;
};

class LowHealthTargetAreaStrategy : public AreaStrategy
{
public:
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const override;
};

class ConflictStrategy
{
public:
    virtual ~ConflictStrategy() = default;

    virtual bool resolveConflict(const std::shared_ptr<Effect>& existingEffect,
                                const std::shared_ptr<Effect>& newEffect) const = 0;
};


class ReplaceConflictStrategy : public ConflictStrategy
{
public:
    bool resolveConflict(const std::shared_ptr<Effect>& existingEffect,
                         const std::shared_ptr<Effect>& newEffect) const override;
};

class ExtendConflictStrategy : public ConflictStrategy
{
public:
    bool resolveConflict(const std::shared_ptr<Effect>& existingEffect,
                        const std::shared_ptr<Effect>& newEffect) const override;
};

class Effect
{
public:
    enum class TriggerType
    {
            INSTANT,
            PASSIVE,
            ON_TURN_START,
            ON_TURN_END,
            ON_DAMAGE,
            ON_AFTER_DAMAGE,
            ON_DEAL_DAMAGE,
            ON_DEATH
    };

    Effect(const std::string& name, int duration,
          std::shared_ptr<AreaStrategy> areaStrategy = std::make_shared<AllTargetsAreaStrategy>(),
          std::shared_ptr<ConflictStrategy> conflictStrategy = std::make_shared<ReplaceConflictStrategy>());
    virtual ~Effect() = default;

    virtual TriggerType getTriggerType() const = 0;
    virtual void apply(Personage* target) = 0;
    virtual void remove(Personage* target){}

    void decreaseDuration();
    std::vector<Personage*> getTargets(Personage* user, const std::vector<Personage*>& allTargets,
                                      const std::vector<int>& rangeZones) const;
    bool resolveConflict(const std::shared_ptr<Effect>& otherEffect) const;

    std::string name;
    int duration;
    std::shared_ptr<AreaStrategy> areaStrategy;
    std::shared_ptr<ConflictStrategy> conflictStrategy;
};

class HealingEffect : public Effect
{
public:
    HealingEffect(int value, int duration,
                 std::shared_ptr<AreaStrategy> areaStrategy = std::make_shared<SingleTargetAreaStrategy>(),
                 std::shared_ptr<ConflictStrategy> conflictStrategy = std::make_shared<ReplaceConflictStrategy>())
        : Effect("Healing", duration, areaStrategy, conflictStrategy), value(value){}

    TriggerType getTriggerType() const override
    {
        return TriggerType::INSTANT;
    }
    void apply(Personage* target) override;

private:
    int value;
};

class DamageEffect : public Effect
{
public:
    DamageEffect(int value, int duration,
                std::shared_ptr<AreaStrategy> areaStrategy = std::make_shared<SingleTargetAreaStrategy>(),
                std::shared_ptr<ConflictStrategy> conflictStrategy = std::make_shared<ReplaceConflictStrategy>())
        : Effect("Damage", duration, areaStrategy, conflictStrategy), value(value){}

    TriggerType getTriggerType() const override
    {
    return TriggerType::INSTANT;
    }
    void apply(Personage* target) override;

private:
    int value;
};

class DamageBoostEffect : public Effect
{
public:
    DamageBoostEffect(int value, int duration,
                     std::shared_ptr<AreaStrategy> areaStrategy = std::make_shared<SingleTargetAreaStrategy>(),
                     std::shared_ptr<ConflictStrategy> conflictStrategy = std::make_shared<ReplaceConflictStrategy>())
        : Effect("Damage Boost", duration, areaStrategy, conflictStrategy), value(value){}

    TriggerType getTriggerType() const override
    {
        return TriggerType::PASSIVE;
    }
    void apply(Personage* target) override;
    void remove(Personage* target) override;

private:
    int value;
};


class ShieldEffect : public Effect
{
private:
    int savedHealth;
    bool isFirstTrigger;

public:
    ShieldEffect();
    TriggerType getTriggerType() const override;
    void apply(Personage* target) override;
    void remove(Personage* target) override;
};

#endif
