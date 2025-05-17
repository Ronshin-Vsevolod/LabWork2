/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "LoadGameState.h"
#include "GameManager.h"
#include "PurchaseState.h"
#include "BattleState.h"
#include "MainMenuState.h"
#include "SkullShopState.h"
#include <iostream>

LoadGameState::LoadGameState(GameManager* gameManager)
    : gameManager(gameManager){}

void LoadGameState::enter()
{
    std::cout << "Загрузка сохранения...\n";
    gameManager->loadGame();

    Player& player = gameManager->getPlayer();
    PlayerData* playerData = gameManager->getPlayerData();

    std::cout << "Баланс монет: " << player.playerData->money << "\n";
    std::cout << "Черепы: " << player.playerData->skulls << "\n";
    std::cout << "Здоровье: " << player.health << "/" << player.playerData->maxHP << "\n";

    switch (playerData->currentGameState)
    {
        case GameStateType::BATTLE:
            std::cout << "Переход к боевому этапу...\n";
            gameManager->changeState(new BattleState(gameManager));
            break;

        case GameStateType::PURCHASE:
        default:
            std::cout << "Переход к этапу покупок...\n";
            gameManager->changeState(new PurchaseState(gameManager));
            break;
    }
}

void LoadGameState::exit()
{
    std::cout << "Выход из состояния загрузки.\n";
}

bool LoadGameState::handleInput(const std::string& inputData)
{
        return false;
}
