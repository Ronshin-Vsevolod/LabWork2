#include "GameManager.h"
#include "MainMenuState.h"
#include "SkullShopState.h"
#include "PurchaseState.h"

GameManager::GameManager()
{
    player = new Player(new PlayerData());
    levelManager = new LevelManager();
    currentState = new MainMenuState(this);
}

void GameManager::startGame()
{
    currentState->enter();
}

void GameManager::changeState(GameState* newState)
{
    if (currentState)
    {
        currentState->exit();
        delete currentState;
    }
    currentState = newState;
    currentState->enter();
}

Player* GameManager::getPlayer()
{
    return player;
}

void GameManager::loadGame()
{
    // Загрузка сохранения (заглушка)
    std::cout << "Игра загружена.\n";
}

void GameManager::completeCurrentLevel()
{
    // Завершение уровня (заглушка)
    std::cout << "Уровень завершён.\n";
}

LevelData* GameManager::getCurrentLevelData()
{
    return levelManager->getLevelData();
}

void GameManager::openSkullShop()
{
    changeState(new SkullShopState(this));
}

void GameManager::openPurchaseState()
{
    changeState(new PurchaseState(this));
}