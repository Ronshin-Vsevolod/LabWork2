#ifndef LOAD_GAME_STATE_H
#define LOAD_GAME_STATE_H

#include "GameState.h"
#include "GameManager.h"

class LoadGameState : public GameState
{
public:
    LoadGameState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    void handleInput(const std::string& inputData) override;

private:
    GameManager* gameManager;
};

#endif