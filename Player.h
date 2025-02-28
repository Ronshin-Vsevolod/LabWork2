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
    void swapPreparedSkills(int index1, int index2);
    void removePreparedSkill(int index);

    void resetOnDeath();

private:
    int swapCooldown;
    int swapCooldownTimer;
};

#endif