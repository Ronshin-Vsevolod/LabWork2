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
    std::cout << "Loading save...\n";
    gameManager->loadGame();

    Player& player = gameManager->getPlayer();
    PlayerData* playerData = gameManager->getPlayerData();

    std::cout << "Money balance: " << player.playerData->money << "\n";
    std::cout << "Skulls: " << player.playerData->skulls << "\n";
    std::cout << "Health: " << player.health << "/" << player.playerData->maxHP << "\n";

    switch (playerData->currentGameState)
    {
        case GameStateType::BATTLE:
            std::cout << "Transition to battle stage...\n";
            gameManager->changeState(new BattleState(gameManager));
            break;

        case GameStateType::PURCHASE:
        default:
            std::cout << "Transition to purchase stage...\n";
            gameManager->changeState(new PurchaseState(gameManager));
            break;
    }
}

void LoadGameState::exit()
{
    std::cout << "Exit from load state.\n";
}

bool LoadGameState::handleInput(const std::string& inputData)
{
        return false;
}
