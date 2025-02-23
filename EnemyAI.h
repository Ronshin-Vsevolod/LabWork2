#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "Enemy.h"
#include "Player.h"

class EnemyAI
{
public:
    EnemyAI(Player* player, int fieldSize);

    void makeTurn(Enemy* enemy);

private:
    Player* player;
    int fieldSize;

    void handlePathZero(Enemy* enemy);
    void handlePositivePath(Enemy* enemy);
    void handleUseSkills(Enemy* enemy);
    void handleTurnAround(Enemy* enemy);
    void handleMoveRight(Enemy* enemy);
    void handleMoveLeft(Enemy* enemy);
    bool isPlayerInSkillRange(Enemy* enemy, const std::vector<std::shared_ptr<Skill>>& skills);
    int chooseSkillToPrepare(Enemy* enemy, const std::vector<std::shared_ptr<Skill>>& availableSkills);
};

#endif