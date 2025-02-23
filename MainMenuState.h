#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "GameState.h"
#include "GameManager.h"

class MainMenuState : public GameState
{
public:
    MainMenuState(GameManager* gameManager, bool hasSave = false);
    void enter() override;
    void exit() override;
    void handleInput(const std::string& inputData) override;

private:
    GameManager* gameManager;
    bool hasSave;
};

#endif