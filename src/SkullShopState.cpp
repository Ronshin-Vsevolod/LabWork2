/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "SkullShopState.h"
#include "MainMenuState.h"
#include "Skills.h"
#include <sstream>
#include <optional>

const std::vector<ShopCommandInfo> SkullShopState::availableCommands =
{
    {
        "unlock", "Разблокировать навык", 1
    },
    {
        "show_skills", "Показать доступные навыки", 2
    },
    {
        "back", "Вернуться в главное меню", 0
    }
};

SkullShopState::SkullShopState(GameManager* gameManager)
    : gameManager(gameManager){}

void SkullShopState::enter()
{
    std::cout << "Вход в магазин черепов.\n";
    showAvailableSkills();

    std::string inputData;
    bool shouldExit = false;

    while(!shouldExit)
    {
        printAvailableCommands();
        std::getline(std::cin, inputData);


        shouldExit = handleInput(inputData);
    }
}

void SkullShopState::exit()
{
    std::cout << "Выход из магазина черепов.\n";
}

void SkullShopState::printAvailableCommands() const
{
    std::cout << "\nДоступные команды:\n";
    for (const ShopCommandInfo& cmd : availableCommands)
    {
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nВведите номер команды или её название: ";
}

ShopCommand SkullShopState::parseCommand(const std::string& input)
{
    int number = 0;
    bool isNumber = true;

    for (char c : input)
    {
        if (!std::isdigit(c))
        {
            isNumber = false;
            break;
        }
    }

    if (isNumber && !input.empty())
    {
        number = std::stoi(input);
        for (const ShopCommandInfo& cmd : availableCommands)
        {
            if (cmd.number == number)
            {
                return static_cast<ShopCommand>(number);
            }
        }
    }

    for (const ShopCommandInfo& cmd : availableCommands)
    {
        if (cmd.name == input)
        {
            return static_cast<ShopCommand>(cmd.number);
        }
    }

    return ShopCommand::UNKNOWN;
}

void SkullShopState::showAvailableSkills() const
{
    PlayerData* playerData = gameManager->getPlayerData();
    std::cout << "\nУ вас " << playerData->skulls << " черепов.\n";

    std::cout << "\nВсего разблокировано навыков: " << playerData->unlockedSkills.size() << "\n";

    std::vector<std::pair<std::string, int>> unlockedSkills;
    std::vector<std::pair<std::string, int>> lockedSkills;

    for (const std::pair<const std::string, SkillFactory::Creator>& skillPair : SkillFactory::registry)
    {
        const std::string& skillName = skillPair.first;
        std::shared_ptr<Skill> skill = skillPair.second();

        if (playerData->isSkillUnlocked(skillName))
        {
            unlockedSkills.push_back(std::make_pair(skillName, skill->skullCost));
        }
        else
        {
            lockedSkills.push_back(std::make_pair(skillName, skill->skullCost));
        }
    }


    std::cout << "\n==== РАЗБЛОКИРОВАННЫЕ НАВЫКИ ====\n";
    if (unlockedSkills.empty())
    {
        std::cout << "Нет разблокированных навыков.\n";
    }
    else
    {
        for (const auto& pair : unlockedSkills)
        {
            std::cout << pair.first << " - " << pair.second << " черепов [Разблокирован]\n";
        }
    }


    std::cout << "\n==== ДОСТУПНЫЕ ДЛЯ РАЗБЛОКИРОВКИ ====\n";
    if (lockedSkills.empty())
    {
        std::cout << "Все навыки уже разблокированы!\n";
    }
    else
    {
        std::cout << "Для разблокировки введите точное название навыка:\n";
        for (const auto& pair : lockedSkills)
        {
            std::cout << pair.first << " - " << pair.second << " черепов\n";
        }
    }

    std::cout << "\n";
}

void SkullShopState::unlockSkill()
{
    PlayerData* playerData = gameManager->getPlayerData();

    std::cout << "\nДоступные навыки:\n";
    std::vector<std::string> availableSkills;
    int index = 1;

    for (const std::pair<const std::string, SkillFactory::Creator>& skillPair : SkillFactory::registry)
    {
        const std::string& skillName = skillPair.first;
        std::shared_ptr<Skill> skill = skillPair.second();

        if (!playerData->isSkillUnlocked(skillName))
        {
            std::cout << index << ". " << skillName << " - " << skill->skullCost << " черепов\n";
            availableSkills.push_back(skillName);
            index++;
        }
    }

    if (availableSkills.empty())
    {
        std::cout << "Все навыки уже разблокированы!\n";
        return;
    }

    std::cout << "\nВведите номер или точное название навыка: ";
    std::string input;
    std::getline(std::cin, input);

    bool isNumber = true;
    for (char c : input)
    {
        if (!std::isdigit(c))
        {
            isNumber = false;
            break;
        }
    }

    std::string skillName;

    if (isNumber && !input.empty())
    {
        int num = std::stoi(input);
        if (num >= 1 && num <= static_cast<int>(availableSkills.size()))
        {
            skillName = availableSkills[num - 1];
        }
        else
        {
            std::cout << "Неверный номер навыка.\n";
            return;
        }
    }
    else
    {
        skillName = input;
    }

    std::unordered_map<std::string, SkillFactory::Creator>::iterator skillIterator = SkillFactory::registry.find(skillName);

    if (skillIterator != SkillFactory::registry.end())
    {
        std::shared_ptr<Skill> skill = skillIterator->second();

        if (playerData->isSkillUnlocked(skillName))
        {
            std::cout << "Навык " << skill->name << " уже разблокирован!\n";
            return;
        }

        if (playerData->skulls >= skill->skullCost)
        {
            playerData->skulls -= skill->skullCost;
            playerData->unlockSkill(skillName);

            gameManager->saveGame();

            std::cout << "Навык " << skill->name << " разблокирован!\n";
            std::cout << "Осталось черепов: " << playerData->skulls << "\n";
        }
        else
        {
            std::cout << "Недостаточно черепов. У вас " << playerData->skulls
                      << ", требуется " << skill->skullCost << ".\n";
        }
    }
    else
    {
        std::cout << "Неизвестный навык. Пожалуйста, введите точное название или номер из списка.\n";
    }
}

bool SkullShopState::handleInput(const std::string& inputData)
{

    ShopCommand command = parseCommand(inputData);

    switch (command)
    {
        case ShopCommand::BACK:
        {
        std::cout << "Возврат в главное меню.\n";

        gameManager->saveGame();

        gameManager->changeState(new MainMenuState(gameManager, true));
        return true;
        }

        case ShopCommand::UNLOCK:
            unlockSkill();
            break;

        case ShopCommand::SHOW_SKILLS:
            showAvailableSkills();
            break;

        case ShopCommand::UNKNOWN:
        default:
            std::cout << "Неизвестная команда.\n";
            break;
    }

    return false;
}
