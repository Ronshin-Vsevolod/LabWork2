/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef PERSONAGE_H
#define PERSONAGE_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class Effect;
class Skill;
class Enemy;
class Player;

class Personage
{
public:
    Personage(const std::string& name, int health, int maxHealth, int location, bool direction);
    virtual ~Personage() = default;

    std::string name;
    int health;
    int maxHealth;
    int damage;
    int location;
    bool direction;
    std::vector<std::shared_ptr<Skill>> skills;
    std::vector<std::shared_ptr<Skill>> prepareStack;

    std::vector<std::shared_ptr<Effect>> effects;

    static std::vector<std::shared_ptr<Personage>> allPersonages;

    virtual void prepareSkill(size_t skillIndex);
    virtual void useSkills(int fieldSize, std::vector<Personage*>& enemies);
    virtual void moveRight(int fieldSize, const std::vector<bool>& occupiedCells);
    virtual void moveLeft(int fieldSize, const std::vector<bool>& occupiedCells);
    virtual void turnAround();
    virtual bool isLocationOccupied(int location, const std::vector<Personage*>& enemies);

    void addEffect(std::shared_ptr<Effect> effect);

    void takeDamage(int damage);
    void dealDamage(Personage* target, int damage);
    void applyEffects();
    const std::string& getName() const {return name;}

    int getHealth() const {return health;}
    int getMaxHealth() const {return maxHealth;}
    int getLocation() const {return location;}
    int getDamage() const {return damage;}

    void setHealth(int newHealth) {health = (newHealth > maxHealth) ? maxHealth : newHealth;}   
    void setLocation(int newLocation) {location = newLocation;}
    void setDamage(int newDamage) {damage = newDamage;}

    void triggerEffect(int triggerType);
    void decreaseEffectsDuration();

protected:
    void checkAllCharacters(std::vector<Personage*>& enemies);
};

#endif
