/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef PLAYER_H
#define PLAYER_H

#include "Personage.h"
#include "PlayerData.h"
#include "EnemyFactory.h"

class Enemy;

class Player : public Personage
{
public:
    Player(PlayerData* playerData);
    void swapWithEnemy(std::vector<std::shared_ptr<Enemy>>& enemies);
    void updateCooldowns();
    void swapPreparedSkills(size_t index1, size_t index2);
    void removePreparedSkill(size_t index);
    PlayerData* playerData;

private:
    int swapCooldown;
    int swapCooldownTimer;
};

#endif
