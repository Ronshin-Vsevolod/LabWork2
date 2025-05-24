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
        "unlock", "Unlock skill", 1
    },
    {
        "show_skills", "Show available skills", 2
    },
    {
        "back", "Return to main menu", 0
    }
};

SkullShopState::SkullShopState(GameManager* gameManager)
    : gameManager(gameManager){}

void SkullShopState::enter()
{
    std::cout << "Enter the skull shop.\n";
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
    std::cout << "Exit from the skull shop.\n";
}

void SkullShopState::printAvailableCommands() const
{
    std::cout << "\nAvailable commands:\n";
    for (const ShopCommandInfo& cmd : availableCommands)
    {
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nEnter the command number or its name: ";
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
    std::cout << "\nYou have " << playerData->skulls << " skulls.\n";

    std::cout << "\nTotal unlocked skills: " << playerData->unlockedSkills.size() << "\n";

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


    std::cout << "\n==== UNLOCKED SKILLS ====\n";
    if (unlockedSkills.empty())
    {
        std::cout << "No unlocked skills.\n";
    }
    else
    {
        for (const auto& pair : unlockedSkills)
        {
            std::cout << pair.first << " - " << pair.second << " skulls [Unlocked]\n";
        }
    }


    std::cout << "\n==== AVAILABLE TO UNLOCK ====\n";
    if (lockedSkills.empty())
    {
        std::cout << "All skills are already unlocked!\n";
    }
    else
    {
        std::cout << "To unlock, enter the exact skill name:\n";
        for (const auto& pair : lockedSkills)
        {
            std::cout << pair.first << " - " << pair.second << " skulls\n";
        }
    }

    std::cout << "\n";
}

void SkullShopState::unlockSkill()
{
    PlayerData* playerData = gameManager->getPlayerData();

    std::cout << "\nAvailable skills:\n";
    std::vector<std::string> availableSkills;
    int index = 1;

    for (const std::pair<const std::string, SkillFactory::Creator>& skillPair : SkillFactory::registry)
    {
        const std::string& skillName = skillPair.first;
        std::shared_ptr<Skill> skill = skillPair.second();

        if (!playerData->isSkillUnlocked(skillName))
        {
            std::cout << index << ". " << skillName << " - " << skill->skullCost << " skulls\n";
            availableSkills.push_back(skillName);
            index++;
        }
    }

    if (availableSkills.empty())
    {
        std::cout << "All skills are already unlocked!\n";
        return;
    }

    std::cout << "\nEnter the skill number or its name: ";
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
            std::cout << "Invalid skill number.\n";
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
            std::cout << "Skill " << skill->name << " already unlocked!\n";
            return;
        }

        if (playerData->skulls >= skill->skullCost)
        {
            playerData->skulls -= skill->skullCost;
            playerData->unlockSkill(skillName);

            gameManager->saveGame();

            std::cout << "Skill " << skill->name << " unlocked!\n";
            std::cout << "Remaining skulls: " << playerData->skulls << "\n";
        }
        else
        {
            std::cout << "Not enough skulls. You have " << playerData->skulls
                      << ", requires " << skill->skullCost << ".\n";
        }
    }
    else
    {
        std::cout << "Unknown skill. Please enter the exact name or number from the list.\n";
    }
}

bool SkullShopState::handleInput(const std::string& inputData)
{

    ShopCommand command = parseCommand(inputData);

    switch (command)
    {
        case ShopCommand::BACK:
        {
        std::cout << "Return to main menu.\n";

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
            std::cout << "Unknown command.\n";
            break;
    }

    return false;
}
