/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include <vector>
#include <memory>

class Player;
class Enemy;
class LevelManager;
class Skill;

class EnemyAI
{
public:
    explicit EnemyAI(int fieldSize);
    virtual ~EnemyAI() = default;

    void makeTurn(Enemy* enemy, Player& player, const LevelManager& levelManager);

protected:
    int fieldSize;
    void handlePathZero(Enemy* enemy, const Player& player, const LevelManager& lm);
    void handlePositivePath(Enemy* enemy);
    void handleUseSkills(Enemy* enemy, Player& player, const LevelManager& levelManager);
    void handleTurnAround(Enemy* enemy, Player& player, const LevelManager& levelManager);
    void handleMoveRight(Enemy* enemy, Player& player, const LevelManager& levelManager);
    void handleMoveLeft(Enemy* enemy, Player& player, const LevelManager& levelManager);

    bool isPlayerInSkillRange(const Skill* skill, int distance) const;
    int chooseSkillToPrepare(Enemy* enemy, const std::vector<std::shared_ptr<Skill>>& availableSkills, const Player& player);
    void resetAllCooldowns(Enemy* enemy);

    bool tryTriggerPreparedSkills(Enemy* enemy, const Player& player);
    bool needsTurnToFacePlayer(Enemy* enemy, const Player& player);
    bool tryPrepareAvailableSkill(Enemy* enemy, const Player& player);
    bool tryMoveTowardPlayer(Enemy* enemy, const LevelManager& lm);
    void handleForcedInaction(Enemy* enemy);
};

#endif
