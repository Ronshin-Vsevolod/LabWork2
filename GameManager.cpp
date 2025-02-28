#include "GameManager.h"
#include "MainMenuState.h"
#include "SkullShopState.h"
#include "PurchaseState.h"
#include "LevelData.h"

GameManager::GameManager()
    : currentState(nullptr), currentLevelIndex(0)
{
    playerData = new PlayerData();
    player = new Player(playerData);

    if (!Levels.empty())
    {
        levelManager = new LevelManager(&Levels[currentLevelIndex]);
    }
    else
    {
        levelManager = new LevelManager(nullptr);
    }
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
    std::cout << "Уровень " << Levels[currentLevelIndex].levelNumber << " завершён!\n";

    player->playerData->skulls += Levels[currentLevelIndex].rewardSkulls;
    std::cout << "Получено черепов: " << Levels[currentLevelIndex].rewardSkulls << "\n";
    std::cout << "Текущее количество черепов: " << player->playerData->skulls << "\n";

    currentLevelIndex++;
    if (currentLevelIndex < Levels.size())
    {
        levelManager = new LevelManager(&Levels[currentLevelIndex]);
        std::cout << "Начинается уровень " << Levels[currentLevelIndex].levelNumber << "!\n";
    }
    else
    {
        std::cout << "Поздравляем! Вы прошли все уровни!\n";
        changeState(new MainMenuState(this));
    }
}

LevelData* GameManager::getCurrentLevelData()
{
    return levelManager->getLevelData();
}

GameState* GameManager::getCurrentState() const
{
    return currentState;
}

void GameManager::openSkullShop()
{
    changeState(new SkullShopState(this));
}

void GameManager::openPurchaseState()
{
    changeState(new PurchaseState(this));
}