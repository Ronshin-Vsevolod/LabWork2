/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef SKULL_SHOP_STATE_H
#define SKULL_SHOP_STATE_H

#include "GameState.h"
#include "GameManager.h"
#include "Skills.h"
#include <iostream>
#include <string>
#include <vector>

struct ShopCommandInfo
{
    std::string name;
    std::string description;
    int number;
};

enum class ShopCommand
{
    UNLOCK = 1,
    SHOW_SKILLS = 2,
    BACK = 0,
    UNKNOWN = -1
};

class SkullShopState : public GameState
{
public:
    SkullShopState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    bool handleInput(const std::string& inputData) override;

private:
    void printAvailableCommands() const;
    static ShopCommand parseCommand(const std::string& input);
    void showAvailableSkills() const;
    void unlockSkill();

    GameManager* gameManager;

    static const std::vector<ShopCommandInfo> availableCommands;
};

#endif
