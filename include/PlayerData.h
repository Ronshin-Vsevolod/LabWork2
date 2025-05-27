/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include "Skills.h"
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_set>

enum class GameStateType
{
        PURCHASE,
        BATTLE,
};

class PlayerData
{
public:
    PlayerData();

    void initDefaultState();
    void saveToFile() const;
    void loadFromFile();

    bool isSkillUnlocked(const std::string& skillName) const;
    void unlockSkill(const std::string& skillName);

    int currentLevel = 0;
    int money;
    int skulls;
    int health;
    int maxHP;
    GameStateType currentGameState = GameStateType::PURCHASE;
    std::vector<std::shared_ptr<Skill>> skills;
    std::unordered_set<std::string> unlockedSkills;

private:
    void initDefaultSkills();
};

#endif
