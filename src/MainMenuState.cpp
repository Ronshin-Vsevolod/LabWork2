/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "MainMenuState.h"
#include "GameManager.h"
#include "BattleState.h"
#include "LoadGameState.h"
#include <iostream>
#include <sstream>
#include <optional>

const std::vector<MenuCommandInfo> MainMenuState::availableCommands =
{
    {
        "new_game", "Start new game", 1
    },
    {
        "continue", "Continue game", 2
    },
    {
        "skull_shop", "Open skull shop", 3
    },
    {
        "exit", "Exit game", 0
    }
};

MainMenuState::MainMenuState(GameManager* gameManager, bool hasSave)
    : gameManager(gameManager), hasSave(hasSave){}

void MainMenuState::enter()
{
    std::cout << "Enter main menu.\n";
    std::string inputData;
    bool shouldExit = false;

    while (!shouldExit)
    {
        printAvailableCommands();
        std::getline(std::cin, inputData);

        shouldExit = handleInput(inputData);
    }
}

void MainMenuState::exit()
{
    std::cout << "Exit from main menu.\n";
}

void MainMenuState::printAvailableCommands() const
{
    std::cout << "\nAvailable commands:\n";
    for (const MenuCommandInfo& cmd : availableCommands)
    {
        if (cmd.name == "continue" && !hasSave)
        {
            continue;
        }
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nEnter the command number or its name: ";
}

MenuCommand MainMenuState::parseCommand(const std::string& input)
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
        for (const MenuCommandInfo& cmd : availableCommands)
        {
            if (cmd.number == number)
            {
                return static_cast<MenuCommand>(number);
            }
        }
    }

    for (const MenuCommandInfo& cmd : availableCommands)
    {
        if (cmd.name == input)
        {
            return static_cast<MenuCommand>(cmd.number);
        }
    }

    return MenuCommand::UNKNOWN;
}

bool MainMenuState::handleInput(const std::string& inputData)
{
    MenuCommand command = parseCommand(inputData);

    switch (command)
    {
        case MenuCommand::EXIT:
            std::cout << "Exit from game.\n";
            return true;

        case MenuCommand::NEW_GAME:
            std::cout << "Start new game.\n";
            gameManager->startNewGame();
            return true;

        case MenuCommand::CONTINUE:
            if (hasSave)
            {
                std::cout << "Continue game.\n";
                gameManager->continueGame();
                return true;
            }
            break;

        case MenuCommand::SKULL_SHOP:
            std::cout << "Opening skull shop.\n";
            std::cout << "You have " << gameManager->getPlayerData()->skulls << " skulls.\n";
            gameManager->openSkullShop();
            return true;

        case MenuCommand::UNKNOWN:
        default:
            std::cout << "Unknown command.\n";
            break;
    }

    return false;
}
