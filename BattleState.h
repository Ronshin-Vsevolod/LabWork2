#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "GameState.h"
#include "GameManager.h"

class BattleState : public GameState
{
public:
    BattleState(class GameManager* gameManager);
    void enter() override;
    void exit() override;
    void handleInput(const std::string& inputData) override;

private:
    void placePlayerInCenter();
    void handleEnemyTurns();
    void checkEnemies();

    GameManager* gameManager;
    LevelManager* levelManager;
    int currentTurn;
    bool isPlayerTurn;
};

#endif