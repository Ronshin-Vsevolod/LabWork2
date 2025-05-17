/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "GameState.h"
#include "GameManager.h"
#include <string>
#include <vector>

struct MenuCommandInfo
{
    std::string name;
    std::string description;
    int number;
};

enum class MenuCommand
{
    NEW_GAME = 1,
    CONTINUE = 2,
    SKULL_SHOP = 3,
    EXIT = 0,
    UNKNOWN = -1
};

class MainMenuState : public GameState
{
public:
    MainMenuState(GameManager* gameManager, bool hasSave = false);
    void enter() override;
    void exit() override;
    bool handleInput(const std::string& inputData) override;

private:
    void printAvailableCommands() const;
    MenuCommand parseCommand(const std::string& input);

    GameManager* gameManager;
    bool hasSave;

    static const std::vector<MenuCommandInfo> availableCommands;
};

#endif
