#include "BattleState.h"
#include "GameManager.h"
#include "LevelManager.h"
#include <iostream>

BattleState::BattleState(GameManager* gameManager)
    : gameManager(gameManager), currentTurn(0), isPlayerTurn(true)
{
    levelManager = new LevelManager(gameManager->getCurrentLevelData());
}

void BattleState::enter()
{
    std::cout << "Вход в этап битвы.\n";
    std::cout << "Текущий уровень: " << levelManager->getLevelData()->levelNumber << "\n";
    placePlayerInCenter();
    isPlayerTurn = true;
}

void BattleState::exit()
{
    std::cout << "Выход из этапа битвы.\n";
}

void BattleState::handleInput(const std::string& inputData)
{
    if (isPlayerTurn)
    {
        if (inputData == "attack")
        {
            std::cout << "Использование навыка.\n";
            gameManager->getPlayer()->useSkills(levelManager->getLevelData()->fieldSize, levelManager->getEnemies());
        }
        else if (inputData == "move_right")
        {
            gameManager->getPlayer()->moveRight(levelManager->getLevelData()->fieldSize, levelManager->getEnemies());
        }
        else if (inputData == "move_left")
        {
            gameManager->getPlayer()->moveLeft(levelManager->getLevelData()->fieldSize, levelManager->getEnemies());
        }
        else if (inputData == "turn_around")
        {
            gameManager->getPlayer()->turnAround();
        }
        else if (inputData == "prepare_skill")
        {
            int skillIndex;
            std::cout << "Введите индекс навыка: ";
            std::cin >> skillIndex;
            gameManager->getPlayer()->prepareSkill(skillIndex);
        }
        else if (inputData == "swap")
        {
            gameManager->getPlayer()->swapWithEnemy(levelManager->getEnemies());
        }
        else if (inputData == "back")
        {
            std::cout << "Возврат в главное меню.\n";
            gameManager->changeState(new MainMenuState(gameManager));
        }

        gameManager->getPlayer()->updateCooldowns();

        isPlayerTurn = false;
        handleEnemyTurns();
    }
}

void BattleState::placePlayerInCenter()
{
    levelManager->placePlayer(gameManager->getPlayer());
}

void BattleState::handleEnemyTurns()
{
    levelManager->removeDeadEnemies();

    for (auto enemy : levelManager->getEnemies())
    {
        if (enemy)
        {
            enemy->makeTurn();
        }
    }

    isPlayerTurn = true;
    currentTurn++;
    levelManager->spawnEnemies(currentTurn, gameManager->getPlayer());
}

void BattleState::checkEnemies()
{
    if (levelManager->areAllEnemiesDefeated())
    {
        std::cout << "Все противники повержены!\n";
        gameManager->completeCurrentLevel();
    }
}