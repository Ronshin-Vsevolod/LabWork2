#ifndef PURCHASE_STATE_H
#define PURCHASE_STATE_H

#include "GameState.h"
#include "GameManager.h"
#include "Skills.h"
#include <iostream>

class PurchaseState : public GameState
{
public:
    PurchaseState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    void handleInput(const std::string& inputData) override;

private:
    GameManager* gameManager;
    Player* player;

    void upgradeSkill();
    void increaseHP();
    void increaseMaxHP();
};

#endif