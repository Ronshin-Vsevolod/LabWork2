#ifndef PLAYER_H
#define PLAYER_H

#include "Personage.h"
#include "PlayerData.h"

class Player : public Personage
{
public:
    Player(PlayerData* playerData);
    void swapWithEnemy(std::vector<Personage*>& enemies);
    void updateCooldowns();
    void swapPreparedSkills(size_t index1, size_t index2);
    void removePreparedSkill(size_t index);
    PlayerData* playerData;

private:
    int swapCooldown;
    int swapCooldownTimer;
};

#endif