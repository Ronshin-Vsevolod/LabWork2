#include "LoadGameState.h"
#include <iostream>

LoadGameState::LoadGameState(GameManager* gameManager)
    : gameManager(gameManager) {}

void LoadGameState::enter()
{
    std::cout << "Загрузка сохранения...\n";
    gameManager->loadGame();
    std::cout << "Баланс монет: " << gameManager->getPlayer()->playerData->money << "\n";
    std::cout << "Черепы: " << gameManager->getPlayer()->playerData->skulls << "\n";
    std::cout << "Здоровье: " << gameManager->getPlayer()->playerData->health << "/" << gameManager->getPlayer()->playerData->maxHP << "\n";
    gameManager->changeState(new BattleState(gameManager));
}

void LoadGameState::exit()
{
    std::cout << "Выход из состояния загрузки.\n";
}

void LoadGameState::handleInput(const std::string& inputData){}