/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef PURCHASE_STATE_H
#define PURCHASE_STATE_H

#include "GameState.h"
#include "GameManager.h"
#include "Skills.h"
#include <iostream>
#include <string>
#include <vector>

class GameManager;
class Player;

struct PurchaseCommandInfo
{
    std::string name;
    std::string description;
    int number;
};

enum class PurchaseCommand
{
    UNKNOWN = -1,
    BACK = 0,
    UPGRADE_SKILL = 1,
    INCREASE_HP = 2,
    INCREASE_MAX_HP = 3,
    START_BATTLE = 4,
    BUY_SKILL = 5
};

class PurchaseState : public GameState
{
public:
    PurchaseState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    bool handleInput(const std::string& inputData) override;

private:
    void printAvailableCommands() const;
    PurchaseCommand parseCommand(const std::string& input);
    void upgradeSkill(Player& player, std::string input);
    void increaseHP();
    void increaseMaxHP();
    void buySkill();

    GameManager* gameManager;
    bool skillPurchased = false;

    static const std::vector<PurchaseCommandInfo> availableCommands;
};

#endif
