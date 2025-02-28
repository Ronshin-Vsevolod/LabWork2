#include "LoadGameState.h"
#include "PurchaseState.h"
#include <iostream>

LoadGameState::LoadGameState(GameManager* gameManager)
    : gameManager(gameManager)
{
}

void LoadGameState::enter()
{
    std::cout << "Загрузка сохранения...\n";
    gameManager->loadGame();

    Player* player = gameManager->getPlayer();
    std::cout << "Баланс монет: " << player->playerData->money << "\n";
    std::cout << "Черепы: " << player->playerData->skulls << "\n";
    std::cout << "Здоровье: " << player->health << "/" << player->playerData->maxHP << "\n";

    gameManager->changeState(new PurchaseState(gameManager));
}

void LoadGameState::exit()
{
    std::cout << "Выход из состояния загрузки.\n";
}

void LoadGameState::handleInput(const std::string& inputData)
{
}