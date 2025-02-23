#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>

class GameState
{
public:
    virtual ~GameState() = default;
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void handleInput(const std::string& inputData) = 0;
};

#endif