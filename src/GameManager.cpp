/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "GameManager.h"
#include "MainMenuState.h"
#include "SkullShopState.h"
#include "PurchaseState.h"
#include "LevelData.h"
#include "BattleState.h"
#include "LoadGameState.h"

GameManager::GameManager()
    : currentState(nullptr),
      currentLevelIndex(0),
      playerData(std::make_unique<PlayerData>()),
      levelManager(std::make_unique<LevelManager>(!Levels.empty() ? &Levels[currentLevelIndex] : nullptr))
{
    player = std::make_unique<Player>(playerData.get());
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

bool GameManager::loadGame()
{
    bool loadSuccess = false;
    std::ifstream testFile("SaveFile");
    if (testFile.is_open())
    {
        testFile.close();

        std::ifstream file("SaveFile");
        int currentLevel, money, skulls, health, maxHP;
        if (file.is_open() && file >> currentLevel >> money >> skulls >> health >> maxHP)
        {
            if (currentLevel >= 0 && money >= 0 && skulls >= 0 && health > 0 && maxHP > 0)
            {
                file.close();
                playerData->loadFromFile();
                loadSuccess = true;
            }
            else
            {
                file.close();
            }
        }
    }

    if (!loadSuccess)
    {
        std::cout << "Save file not found or corrupted. Using default settings.\n";
        playerData->initDefaultState();
    }

    currentLevelIndex = playerData->currentLevel;

    if (currentLevelIndex < Levels.size())
    {
        levelManager = std::make_unique<LevelManager>(&Levels[currentLevelIndex]);
    } else
    {
        currentLevelIndex = 0;
        levelManager = std::make_unique<LevelManager>(&Levels[currentLevelIndex]);
        std::cout << "Warning: incorrect level index. Loaded first level.\n";
    }

    updatePlayer();

    if (loadSuccess)
    {
        std::cout << "Game successfully loaded.\n";
    }

    return loadSuccess;
}

void GameManager::completeCurrentLevel()
{
    player->playerData->currentLevel = currentLevelIndex;
    std::cout << "Level " << Levels[currentLevelIndex].levelNumber << " completed!\n";

    player->playerData->skulls += Levels[currentLevelIndex].rewardSkulls;
    std::cout << "Received skulls: " << Levels[currentLevelIndex].rewardSkulls << "\n";
    std::cout << "Current skulls: " << player->playerData->skulls << "\n";

    player->playerData->money += Levels[currentLevelIndex].rewardMoney;
    std::cout << "Received money: " << Levels[currentLevelIndex].rewardMoney << "\n";
    std::cout << "Current money: " << player->playerData->money << "\n";

    currentLevelIndex++;

    player->playerData->currentGameState = GameStateType::PURCHASE;

    saveGame();

    if (currentLevelIndex < Levels.size())
    {
        resetLevelManager(currentLevelIndex);
        std::cout << "Level " << Levels[currentLevelIndex].levelNumber << " started!\n";

        updatePlayer();

        changeState(new PurchaseState(this));
    }
    else
    {
        std::cout << "Congratulations! You passed all levels!\n";
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

void GameManager::startNewGame()
{
    playerData->initDefaultState();

    currentLevelIndex = 0;
    levelManager = std::make_unique<LevelManager>(&Levels[currentLevelIndex]);
    updatePlayer();

    playerData->currentGameState = GameStateType::BATTLE;

    saveGame();

    changeState(new BattleState(this));
}

void GameManager::continueGame()
{
    changeState(new LoadGameState(this));
}

void GameManager::saveGame()
{
    playerData->currentLevel = currentLevelIndex;

    if (player)
    {
        playerData->health = player->health;
        playerData->skills = player->skills;
    }

    playerData->saveToFile();
}

void GameManager::updatePlayer()
{
    player = std::make_unique<Player>(playerData.get());

    player->health = playerData->health;
    player->skills = playerData->skills;
}
