/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "PurchaseState.h"
#include "BattleState.h"
#include "MainMenuState.h"
#include <sstream>
#include "GameManager.h"
#include "Player.h"
#include <iostream>
#include <optional>
#include <random>


const std::vector<PurchaseCommandInfo> PurchaseState::availableCommands = {
    {
        "upgrade", "Upgrade skill", 1},
    {
        "increase_hp", "Restore health", 2},
    {
        "increase_max_hp", "Increase maximum health", 3},
    {
        "start_battle", "Start battle", 4},
    {
        "buy_skill", "Buy skill", 5},
    {
        "back", "Return to main menu", 0}
    };

PurchaseState::PurchaseState(GameManager* gameManager)
    : gameManager(gameManager), skillPurchased(false){}

void PurchaseState::enter()
{
    std::cout << "Enter purchase stage.\n";
    skillPurchased = false;
    Player& player = gameManager->getPlayer();
    std::string inputData;
    bool shouldExit = false;

    while (!shouldExit)
    {
        std::cout << "\nCurrent money: " << player.playerData->money << "\n";
        std::cout << "Current health: " << player.health << "/" << player.playerData->maxHP << "\n";

        printAvailableCommands();
        std::getline(std::cin, inputData);

        shouldExit = handleInput(inputData);
    }
}

void PurchaseState::exit()
{
    std::cout << "Exit from purchase stage.\n";
}

void PurchaseState::printAvailableCommands() const
{
    std::cout << "\nAvailable commands:\n";
    for (const PurchaseCommandInfo& cmd : availableCommands)
    {
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nEnter the command number or its name: ";
}

PurchaseCommand PurchaseState::parseCommand(const std::string& input)
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
        for (const PurchaseCommandInfo& cmd : availableCommands)
        {
            if (cmd.number == number)
            {
                return static_cast<PurchaseCommand>(number);
            }
        }
    }

    for (const PurchaseCommandInfo& cmd : availableCommands)
    {
        if (cmd.name == input)
        {
            return static_cast<PurchaseCommand>(cmd.number);
        }
    }

    return PurchaseCommand::UNKNOWN;
}

bool PurchaseState::handleInput(const std::string& inputData)
{
    Player& player = gameManager->getPlayer();
    PurchaseCommand command = parseCommand(inputData);

    switch (command)
    {
        case PurchaseCommand::BACK:
        {
            std::cout << "Return to main menu.\n";
            gameManager->changeState(new MainMenuState(gameManager, true));
            return true;
        }

        case PurchaseCommand::UPGRADE_SKILL:
        {
            Player& player = gameManager->getPlayer();

            std::cout << "\nYour skills:\n";
            for (size_t i = 0; i < player.playerData->skills.size(); ++i)
            {
                const std::shared_ptr<Skill>& skill = player.playerData->skills[i];
                std::cout << i + 1 << ". " << skill->name << "\n";
            }

            if (player.playerData->skills.empty())
            {
                std::cout << "You don't have skills to upgrade!\n";
                return false;
            }

            std::cout << "Enter the skill number to upgrade: ";
            std::string input;
            std::getline(std::cin, input);

            upgradeSkill(player, input);
            break;
        }

        case PurchaseCommand::INCREASE_HP:
        {
            int cost = 2;
            if (player.playerData->money < cost)
            {
                std::cout << "Not enough money for healing (requires " << cost << ").\n";
                return false;
            }

            if (player.health >= player.playerData->maxHP)
            {
                std::cout << "Health is already at maximum!\n";
                return false;
            }

            player.playerData->money -= cost;
            player.health = player.playerData->maxHP;

            std::cout << "Health fully restored: "
                      << player.health << "/" << player.playerData->maxHP << "\n";
            break;
        }

        case PurchaseCommand::INCREASE_MAX_HP:
        {
            increaseMaxHP();
            break;
        }

        case PurchaseCommand::BUY_SKILL:
        {
            buySkill();
            break;
        }

        case PurchaseCommand::START_BATTLE:
        {  
            std::cout << "Transition to battle stage.\n";


            gameManager->getPlayerData()->currentGameState = GameStateType::BATTLE;


            gameManager->saveGame();

            gameManager->changeState(new BattleState(gameManager));
            return true;
        }

        case PurchaseCommand::UNKNOWN:  
        default:
        {
            std::cout << "Unknown command.\n";
            break;
        }
    }
    return false;
}

void PurchaseState::upgradeSkill(Player& player, std::string input)
{
    bool isValidNumber = true;
    if (input.empty()) isValidNumber = false;

    for (char c : input)
    {
        if (!std::isdigit(c))
        {
            isValidNumber = false;
            break;
        }
    }

    if (!isValidNumber)
    {
        std::cout << "Error: enter a valid skill number!\n";
        return;
    }

    size_t choice = std::stoul(input);

    if (choice < 1 || choice > player.playerData->skills.size())
    {
        std::cout << "Error: enter a valid skill number!\n";
        return;
    }

    std::shared_ptr<Skill> selectedSkill = player.playerData->skills[choice - 1];
    ClassicSkill* classicSkill = dynamic_cast<ClassicSkill*>(selectedSkill.get());
    if (!classicSkill)
    {
        std::cout << "This skill can't be upgraded yet!\n";
        return;
    }

    if (player.playerData->money >= selectedSkill->coinCost)
    {
        player.playerData->money -= selectedSkill->coinCost;
        classicSkill->upgrade();
        std::cout << "Skill " << selectedSkill->name
                  << " upgraded! New damage: " << classicSkill->damage << "\n";
    }
    else
    {
        std::cout << "Not enough money. Need: " << selectedSkill->coinCost
                  << ", have: " << player.playerData->money << "\n";
    }
}

void PurchaseState::increaseHP()
{
    Player& player = gameManager->getPlayer();
    if (player.playerData->money >= 2)
    {
        player.playerData->money -= 2;
        player.playerData->health += 1;
        std::cout << "HP increased!\n";
    }
    else
    {
        std::cout << "Not enough money to increase HP.\n";
    }
}

void PurchaseState::increaseMaxHP()
{
    Player& player = gameManager->getPlayer();
    int cost = 3;

    if (player.playerData->money < cost)
    {
        std::cout << "Not enough money to increase maximum health (requires " << cost << ").\n";
        return;
    }

    player.playerData->money -= cost;

    int increase = 1;
    player.playerData->maxHP += increase;

    std::cout << "Maximum health increased by " << increase << ". Current health: "
              << player.health << "/" << player.playerData->maxHP << "\n";
}

void PurchaseState::buySkill()
{
    Player& player = gameManager->getPlayer();
    PlayerData* playerData = gameManager->getPlayerData();

    if (skillPurchased)
    {
        std::cout << "You already purchased a skill in this visit. Go to battle or perform another action.\n";
        return;
    }

    std::cout << "Available unlocked skills:\n";

    std::vector<std::string> allAvailableSkills;

    for (const std::pair<const std::string, SkillFactory::Creator>& skillPair : SkillFactory::registry)
    {
        const std::string& skillName = skillPair.first;

        if (playerData->isSkillUnlocked(skillName))
        {
            allAvailableSkills.push_back(skillName);
        }
    }

    if (allAvailableSkills.empty())
    {
        std::cout << "No available skills to buy.\n";
        return;
    }

    std::vector<std::string> displayedSkills;
    std::vector<int> displayedPrices;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::string> skillsPool = allAvailableSkills;

    int numToShow = std::min(3, static_cast<int>(skillsPool.size()));

    for (int i = 0; i < numToShow; i++)
    {
        if (skillsPool.empty()) break;

        std::uniform_int_distribution<int> distribution(0, skillsPool.size() - 1);
        int randomIndex = distribution(gen);

        std::string skillName = skillsPool[randomIndex];

        std::shared_ptr<Skill> skill = SkillFactory::create(skillName);

        std::cout << (i + 1) << ". " << skillName << " - " << skill->coinCost << " coins\n";

        displayedSkills.push_back(skillName);
        displayedPrices.push_back(skill->coinCost);

        skillsPool.erase(skillsPool.begin() + randomIndex);
    }

    if (displayedSkills.empty())
    {
        std::cout << "No available skills to buy.\n";
        return;
    }

    std::cout << "Enter the skill number to buy (you can buy only one skill at a time): ";
    std::string input;
    std::getline(std::cin, input);

    bool isValidNumber = true;
    for (char c : input)
    {
        if (!std::isdigit(c))
        {
            isValidNumber = false;
            break;
        }
    }

    if (!isValidNumber || input.empty())
    {
        std::cout << "Error: enter a valid skill number!\n";
        return;
    }

    size_t choice = std::stoul(input);
    if (choice < 1 || choice > displayedSkills.size())
    {
        std::cout << "Error: enter a valid skill number!\n";
        return;
    }

    std::string selectedSkillName = displayedSkills[choice - 1];
    int skillCost = displayedPrices[choice - 1];

    if (player.playerData->money >= skillCost)
    {
        player.playerData->money -= skillCost;
        player.skills.push_back(SkillFactory::create(selectedSkillName));
        std::cout << "Skill '" << selectedSkillName << "' successfully purchased!\n";
            skillPurchased = true;
    }
    else
    {
        std::cout << "Not enough money to buy skill (requires "
                  << skillCost << ", you have " << player.playerData->money << ").\n";
    }
}
