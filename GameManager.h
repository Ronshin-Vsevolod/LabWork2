#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameState.h"
#include "Player.h"
#include "LevelManager.h"

class GameManager
{
public:
    GameManager();
    void changeState(GameState* newState);
    Player* getPlayer();
    void loadGame();
    void completeCurrentLevel();
    LevelData* getCurrentLevelData();
    GameState* getCurrentState() const;
    void openSkullShop();
    void openPurchaseState();
    
private:
    GameState* currentState;
    Player* player;
    LevelManager* levelManager;
    size_t currentLevelIndex;
    PlayerData* playerData;
};

#endif