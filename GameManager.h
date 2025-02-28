#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameState.h"
#include "Player.h"
#include "LevelManager.h"

class GameManager
{
public:
    GameManager();
    void startGame();
    void changeState(GameState* newState);
    Player* getPlayer();
    void loadGame();
    void completeCurrentLevel();
    LevelData* getCurrentLevelData();
    void openSkullShop();
    void openPurchaseState();

private:
    GameState* currentState;
    Player* player;
    LevelManager* levelManager;
};

#endif