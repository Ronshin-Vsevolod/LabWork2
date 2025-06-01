/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef LOAD_GAME_STATE_H
#define LOAD_GAME_STATE_H

#include "GameState.h"

class GameManager;

class LoadGameState : public GameState
{
public:
    LoadGameState(GameManager* gameManager);
    void enter() override;
    void exit() override;
    bool handleInput(const std::string& inputData) override;

private:
    GameManager* gameManager;
};

#endif
