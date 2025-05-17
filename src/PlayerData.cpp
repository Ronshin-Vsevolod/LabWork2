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
        std::cerr << "Ошибка: не удалось открыть файл для записи.\n";
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

    std::cout << "Игра сохранена: " << unlockedSkills.size() << " разблокированных навыков\n";
    file.close();
}


void PlayerData::loadFromFile()
{
    const std::string filename = "SaveFile";
    std::ifstream file(filename);

    if (file.is_open())
    {
        std::cout << "Открытие файла сохранения...\n";

        try
        {
            std::string line;

            if (!std::getline(file, line) || !(std::stringstream(line) >> currentLevel))
            {
                std::cerr << "Ошибка при чтении уровня\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> money))
            {
                std::cerr << "Ошибка при чтении денег\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> skulls))
            {
                std::cerr << "Ошибка при чтении черепов\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> health))
            {
                std::cerr << "Ошибка при чтении здоровья\n";
                initDefaultState();
                return;
            }

            if (!std::getline(file, line) || !(std::stringstream(line) >> maxHP))
            {
                std::cerr << "Ошибка при чтении максимального здоровья\n";
                initDefaultState();
                return;
            }


            int gameStateValue = 0;
            if (!std::getline(file, line) || !(std::stringstream(line) >> gameStateValue))
            {
                std::cerr << "Ошибка при чтении текущего состояния игры\n";
                initDefaultState();
                return;
            }
            else
            {
                currentGameState = static_cast<GameStateType>(gameStateValue);
            }

            std::cout << "Базовые параметры загружены: уровень=" << currentLevel
                      << ", деньги=" << money << ", черепа=" << skulls
                      << ", состояние=" << static_cast<int>(currentGameState) << "\n";

            size_t skillCount = 0;
            if (!std::getline(file, line) || !(std::stringstream(line) >> skillCount))
            {
                std::cerr << "Предупреждение: не удалось прочитать количество навыков\n";
                initDefaultState();
                return;
            }

            std::cout << "Найдено " << skillCount << " навыков для загрузки\n";
            skills.clear();

            for (size_t i = 0; i < skillCount; ++i)
            {
                std::string skillName;
                if (!std::getline(file, skillName))
                {
                    std::cerr << "Предупреждение: не удалось прочитать навык #" << i + 1 << "\n";
                    initDefaultState();
                    return;
                }

                std::cout << "Загружен навык: \"" << skillName << "\"\n";
                if (SkillFactory::registry.find(skillName) != SkillFactory::registry.end())
                {
                    skills.push_back(SkillFactory::registry[skillName]());
                }
                else
                {
                    std::cerr << "Предупреждение: навык '" << skillName << "' не найден в реестре\n";
                }
            }

            size_t unlockedCount = 0;
            if (!std::getline(file, line) || !(std::stringstream(line) >> unlockedCount))
            {
                std::cerr << "Предупреждение: не удалось прочитать количество разблокированных навыков\n";

                unlockedSkills.clear();
                unlockedSkills.insert("Katana");
                unlockedSkills.insert("Two katanas");
                unlockedSkills.insert("Holy Grenade");
                unlockedSkills.insert("Yari");
            }
            else
            {
                std::cout << "Найдено " << unlockedCount << " разблокированных навыков\n";

                unlockedSkills.clear();

                for (size_t i = 0; i < unlockedCount; ++i)
                {
                    std::string skillName;
                    if (!std::getline(file, skillName))
                    {
                        std::cerr << "Предупреждение: не удалось прочитать разблокированный навык #" << i + 1 << "\n";
                        continue;
                    }

                    std::cout << "Загружен разблокированный навык: \"" << skillName << "\"\n";


                    if (SkillFactory::registry.find(skillName) != SkillFactory::registry.end())
                    {
                        unlockedSkills.insert(skillName);
                    } 
                    else
                    {
                        std::cerr << "Предупреждение: навык '" << skillName << "' не найден в реестре и не был добавлен\n";
                    }
                }
                
                unlockedSkills.insert("Katana");
                unlockedSkills.insert("Two katanas");
                unlockedSkills.insert("Holy Grenade");
                unlockedSkills.insert("Yari");

                std::cout << "Итого загружено " << unlockedSkills.size() << " разблокированных навыков\n";
            }


            if (skills.empty())
            {
                std::cerr << "Предупреждение: не загружено ни одного навыка. Загружены навыки по умолчанию\n";

                skills.clear();
                initDefaultSkills();
            }

            std::cout << "Сохранение успешно загружено\n";
        }
        catch (const std::exception& e)
        {
            std::cerr << "Ошибка при загрузке сохранения: " << e.what() << "\n";
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
