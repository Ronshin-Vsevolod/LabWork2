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
        "new_game", "Начать новую игру", 1
    },
    {
        "continue", "Продолжить игру", 2
    },
    {
        "skull_shop", "Открыть магазин черепов", 3
    },
    {
        "exit", "Выйти из игры", 0
    }
};

MainMenuState::MainMenuState(GameManager* gameManager, bool hasSave)
    : gameManager(gameManager), hasSave(hasSave){}

void MainMenuState::enter()
{
    std::cout << "Вход в главное меню.\n";
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
    std::cout << "Выход из главного меню.\n";
}

void MainMenuState::printAvailableCommands() const
{
    std::cout << "\nДоступные команды:\n";
    for (const MenuCommandInfo& cmd : availableCommands)
    {
        if (cmd.name == "continue" && !hasSave)
        {
            continue;
        }
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nВведите номер команды или её название: ";
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
            std::cout << "Выход из игры.\n";
            return true;

        case MenuCommand::NEW_GAME:
            std::cout << "Начало новой игры.\n";
            gameManager->startNewGame();
            return true;

        case MenuCommand::CONTINUE:
            if (hasSave)
            {
                std::cout << "Продолжение игры.\n";
                gameManager->continueGame();
                return true;
            }
            break;

        case MenuCommand::SKULL_SHOP:
            std::cout << "Открытие магазина черепов.\n";
            std::cout << "У вас " << gameManager->getPlayerData()->skulls << " черепов.\n";
            gameManager->openSkullShop();
            return true;

        case MenuCommand::UNKNOWN:
        default:
            std::cout << "Неизвестная команда.\n";
            break;
    }

    return false;
}
