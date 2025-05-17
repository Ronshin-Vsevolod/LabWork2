/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>

class GameState
{
public:
    virtual ~GameState() = default;
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual bool handleInput(const std::string& inputData) = 0;
};
#endif
