/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "PlayerData.h"
#include <iostream>
#include <fstream>
#include <sstream>

PlayerData::PlayerData()
    : money(0), skulls(0), health(6), maxHP(6)
{
    skills.push_back(SkillFactory::create("Katana"));
    skills.push_back(SkillFactory::create("Two katanas"));

    skills.push_back(SkillFactory::create("Holy Grenade"));
}

bool PlayerData::isSkillUnlocked(const std::string& skillName) const
{
    if (skillName == "Katana" || skillName == "Two katanas" || skillName == "Holy Grenade" || skillName == "Yari")
    {
        return true;
    }

    return unlockedSkills.find(skillName) != unlockedSkills.end();
}

void PlayerData::unlockSkill(const std::string& skillName)
{
    unlockedSkills.insert(skillName);
}

void PlayerData::initDefaultState()
{
    currentLevel = 0;
    money = 0;
    health = 6;
    maxHP = 6;
    currentGameState = GameStateType::BATTLE;

    skills.clear();
    skills =
    {
        SkillFactory::create("Katana"),
        SkillFactory::create("Two katanas"),
        SkillFactory::create("Holy Grenade")
    };

    if (unlockedSkills.empty())
    {
        unlockedSkills.insert("Katana");
        unlockedSkills.insert("Two katanas");
        unlockedSkills.insert("Holy Grenade");
        unlockedSkills.insert("Yari");
    }
}

void PlayerData::saveToFile() const
{
    const std::string filename = "SaveFile";
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: failed to open file for writing.\n";
        return;
    }

    file << currentLevel << std::endl;
    file << money << std::endl;
    file << skulls << std::endl;
    file << health << std::endl;
    file << maxHP << std::endl;
    file << static_cast<int>(currentGameState) << std::endl;
    file << skills.size() << std::endl;

    for (const std::shared_ptr<Skill>& skill : skills)
    {
        file << skill->name << std::endl;
    }

    file << unlockedSkills.size() << std::endl;
    for (const std::string& skillName : unlockedSkills)
    {
        file << skillName << std::endl;
    }

    std::cout << "Game saved: " << unlockedSkills.size() << " unlocked skills\n";
    file.close();
}


void PlayerData::loadFromFile()
{
    const std::string filename = "SaveFile";
    std::ifstream file(filename);

    if (file.is_open())
    {
        std::cout << "Opening save file...\n";

        try
        {
            std::string line;

            if (!std::getline(file, line) || !(std::stringstream(line) >> currentLevel))
            {
                std::cerr << "Error reading level\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> money))
            {
                std::cerr << "Error reading money\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> skulls))
            {
                std::cerr << "Error reading skulls\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> health))
            {
                std::cerr << "Error reading health\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> maxHP))
            {
                std::cerr << "Error reading maximum health\n";
                initDefaultState();
                return;
            }


            int gameStateValue = 0;
            if (!std::getline(file, line) || !(std::stringstream(line) >> gameStateValue))
            {
                std::cerr << "Error reading current game state\n";
                initDefaultState();
                return;
            }
            else
            {
                currentGameState = static_cast<GameStateType>(gameStateValue);
            }

            std::cout << "Base parameters loaded: level=" << currentLevel
                      << ", money=" << money << ", skulls=" << skulls
                      << ", state=" << static_cast<int>(currentGameState) << "\n";

            size_t skillCount = 0;
            if (!std::getline(file, line) || !(std::stringstream(line) >> skillCount))
            {
                std::cerr << "Warning: failed to read skill count\n";
                initDefaultState();
                return;
            }

            std::cout << "Found " << skillCount << " skills to load\n";
            skills.clear();

            for (size_t i = 0; i < skillCount; ++i)
            {
                std::string skillName;
                if (!std::getline(file, skillName))
                {
                    std::cerr << "Warning: failed to read skill #" << i + 1 << "\n";
                    initDefaultState();
                    return;
                }

                std::cout << "Loaded skill: \"" << skillName << "\"\n";
                if (SkillFactory::registry.find(skillName) != SkillFactory::registry.end())
                {
                    skills.push_back(SkillFactory::registry[skillName]());
                }
                else
                {
                    std::cerr << "Warning: skill '" << skillName << "' not found in registry\n";
                }
            }

            size_t unlockedCount = 0;
            if (!std::getline(file, line) || !(std::stringstream(line) >> unlockedCount))
            {
                std::cerr << "Warning: failed to read number of unlocked skills\n";

                unlockedSkills.clear();
                unlockedSkills.insert("Katana");
                unlockedSkills.insert("Two katanas");
                unlockedSkills.insert("Holy Grenade");
                unlockedSkills.insert("Yari");
            }
            else
            {
                std::cout << "Found " << unlockedCount << " unlocked skills\n";

                unlockedSkills.clear();

                for (size_t i = 0; i < unlockedCount; ++i)
                {
                    std::string skillName;
                    if (!std::getline(file, skillName))
                    {
                        std::cerr << "Warning: failed to read unlocked skill #" << i + 1 << "\n";
                        continue;
                    }

                    std::cout << "Loaded unlocked skill: \"" << skillName << "\"\n";


                    if (SkillFactory::registry.find(skillName) != SkillFactory::registry.end())
                    {
                        unlockedSkills.insert(skillName);
                    } 
                    else
                    {
                        std::cerr << "Warning: skill '" << skillName << "' not found in registry and not added\n";
                    }
                }
                
                unlockedSkills.insert("Katana");
                unlockedSkills.insert("Two katanas");
                unlockedSkills.insert("Holy Grenade");
                unlockedSkills.insert("Yari");

                std::cout << "Total loaded " << unlockedSkills.size() << " unlocked skills\n";
            }


            if (skills.empty())
            {
                std::cerr << "Warning: no skills loaded. Default skills loaded\n";

                skills.clear();
                initDefaultSkills();
            }

            std::cout << "Save successfully loaded\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error loading save: " << e.what() << "\n";
            initDefaultState();
        }

        file.close();
    }
    else
    {
        initDefaultState();
    }
}

void PlayerData::initDefaultSkills()
{
    skills.push_back(SkillFactory::create("Katana"));
    skills.push_back(SkillFactory::create("Two katanas"));
    skills.push_back(SkillFactory::create("Holy Grenade"));
}
