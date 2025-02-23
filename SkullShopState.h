#ifndef SKULL_SHOP_STATE_H
#define SKULL_SHOP_STATE_H

#include "GameState.h"
#include "GameManager.h"
#include "Skills.h"
#include <iostream>

class SkullShopState : public GameState
{
public:
    SkullShopState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    void handleInput(const std::string& inputData) override;

private:
    GameManager* gameManager;
    Player* player;
};

#endif