#include "MainMenuState.h"
#include "BattleState.h"
#include "LoadGameState.h"
#include <iostream>

MainMenuState::MainMenuState(GameManager* gameManager, bool hasSave)
    : gameManager(gameManager), hasSave(hasSave)
{
}

void MainMenuState::enter()
{
    std::cout << "Вход в главное меню.\n";
    if (hasSave)
    {
        std::cout << "0. Выход\n";
        std::cout << "1. Новая игра\n";
        std::cout << "2. Продолжить\n";
        std::cout << "3. Магазин черепов\n";
        std::cout << "4. Этап покупок\n";
    }
    else
    {
        std::cout << "0. Выход\n";
        std::cout << "1. Новая игра\n";
        std::cout << "3. Магазин черепов\n";
        std::cout << "4. Этап покупок\n";
    }

    std::string inputData;

    while (true)
    {
        std::cin >> inputData;

        handleInput(inputData);

        if (inputData == "0")
        {
            break;
        }
    }
}

void MainMenuState::exit()
{
    std::cout << "Выход из главного меню.\n";
}

void MainMenuState::handleInput(const std::string& inputData)
{
    if (inputData == "0")
    {
        std::cout << "Выход из игры.\n";
        return;
    }
    else if (inputData == "1")
    {
        std::cout << "Начало новой игры.\n";
        gameManager->changeState(new BattleState(gameManager));
    }
    else if (inputData == "2" && hasSave)
    {
        std::cout << "Продолжение игры.\n";
        gameManager->changeState(new LoadGameState(gameManager));
    }
    else if (inputData == "3")
    {
        std::cout << "Открытие магазина черепов.\n";
        gameManager->openSkullShop();
    }
    else if (inputData == "4")
    {
        std::cout << "Открытие этапа покупок.\n";
        gameManager->openPurchaseState();
    }
}