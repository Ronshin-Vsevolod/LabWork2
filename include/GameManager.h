/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameState.h"
#include "Player.h"
#include "LevelManager.h"
#include "LevelData.h"
#include "LoadGameState.h"
#include <memory>

class GameManager
{
public:
    GameManager();
    ~GameManager() = default;

    Player& getPlayer() {return *player;}
    const Player& getPlayer() const {return *player;}
    PlayerData* getPlayerData() {return playerData.get();}
    const PlayerData* getPlayerData() const {return playerData.get();}
    LevelManager& getLevelManager() {return *levelManager;}
    const LevelManager& getLevelManager() const {return *levelManager;}
    LevelData* getCurrentLevelData();
    GameState* getCurrentState() const;
    size_t getCurrentLevelIndex() const {return currentLevelIndex;}

    void resetLevelManager(int levelIndex) {levelManager = std::make_unique<LevelManager>(&Levels[levelIndex]);}
    void changeState(GameState* newState);
    void completeCurrentLevel();
    void openSkullShop();
    void openPurchaseState();

    void startNewGame();
    void continueGame();
    void saveGame();
    bool loadGame();
    void updatePlayer();

private:
    GameState* currentState;
    size_t currentLevelIndex;
    std::unique_ptr<PlayerData> playerData;
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<Player> player;
};

#endif
