#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "GameState.h"
#include "GameManager.h"

class BattleState : public GameState
{
public:
    BattleState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    void handleInput(const std::string& inputData) override;

private:
    void handlePlayerTurn(const std::string& inputData);
    void placePlayerInCenter();
    void handleEnemyTurns();
    void checkEnemies();

    GameManager* gameManager;
    LevelManager* levelManager;
    int currentTurn;
    bool isPlayerTurn;
};

#endif