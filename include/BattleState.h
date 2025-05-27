/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include <vector>
#include <memory>
#include <string>
#include "GameState.h"

class GameManager;
class LevelManager;

enum PlayerCommand
{
    UNKNOWN = 0,
    ATTACK = 1,
    MOVE_LEFT = 2,
    MOVE_RIGHT = 3,
    PREPARE_SKILL = 4,
    SWAP_WITH_ENEMY = 5,
    TURN_AROUND = 6,
    BACK = 7,
    END_TURN = 8
};

struct CommandInfo
{
    std::string name;
    std::string description;
    int number;
};

class BattleState : public GameState
{
private:
    GameManager* gameManager;
    std::unique_ptr<LevelManager> levelManager;
    int currentTurn;
    bool isPlayerTurn;

    static const std::vector<CommandInfo> availableCommands;

public:
    BattleState(GameManager* gameManager);

    void enter() override;
    void exit() override;

    bool handleInput(const std::string& inputData);
    void handleEnemyTurns();

    void printAvailableCommands() const;
    PlayerCommand parseCommand(const std::string& input);
};

#endif
