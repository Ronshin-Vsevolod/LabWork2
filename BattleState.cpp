#include "BattleState.h"
#include "GameManager.h"
#include "LevelManager.h"
#include "MainMenuState.h"
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

    while (true)
    {
        if (isPlayerTurn)
        {
            std::cout << "Ход игрока.\n";
            std::string inputData;
            std::cin >> inputData;
            handlePlayerTurn(inputData);
            isPlayerTurn = false;
        }
        
        else
        {
            std::cout << "Ход противников.\n";
            handleEnemyTurns();
            isPlayerTurn = true;
        }

        if (gameManager->getPlayer()->health <= 0)
        {
            std::cout << "Игрок погиб!\n";
            gameManager->getPlayer()->resetOnDeath();
            break;
        }
        
        if (levelManager->areAllEnemiesDefeated())
        {
            std::cout << "Все противники повержены!\n";
            gameManager->completeCurrentLevel();
            break;
        }
    }
}

void BattleState::exit()
{
    std::cout << "Выход из этапа битвы.\n";
}

void BattleState::handleInput(const std::string& inputData)
{
    handlePlayerTurn(inputData);
}

void BattleState::handlePlayerTurn(const std::string& inputData) {
    Player* player = gameManager->getPlayer();
    bool turnEnded = false;
    std::string input = inputData;

    while (!turnEnded)
    {
        if (input == "attack")
        {
            std::vector<Personage*> enemies;
            for (const auto& enemy : levelManager->getEnemies())
            {
                enemies.push_back(enemy.get());
            }

            if (!player->skills.empty())
            {
                player->skills[0]->applyEffect(player, enemies);
            }
            turnEnded = true;
        }
        else if (input == "move_right")
        {
            turnEnded = true;
        }
        else if (input == "move_left")
        {
            turnEnded = true;
        }
        else if (inputData == "turn_around")
        {
            player->turnAround();
            turnEnded = true;
        }
        else if (inputData == "prepare_skill")
        {
            int skillIndex;
            std::cout << "Введите индекс навыка: ";
            std::cin >> skillIndex;
            player->prepareSkill(skillIndex);
            turnEnded = true;
        }
        else if (inputData == "swap_skills")
        {
            int index1, index2;
            std::cout << "Введите индексы навыков для обмена: ";
            std::cin >> index1 >> index2;
            player->swapPreparedSkills(index1, index2);
        }
        else if (inputData == "remove_skill")
        {
            int index;
            std::cout << "Введите индекс навыка для удаления: ";
            std::cin >> index;
            player->removePreparedSkill(index);
        }
        else if (inputData == "back")
        {
            std::cout << "Возврат в главное меню.\n";
            gameManager->changeState(new MainMenuState(gameManager));
            turnEnded = true;
        }
        else
        {
            std::cout << "Неизвестная команда.\n";
        }

        if (!turnEnded)
        {
            std::cout << "Введите следующую команду: ";
            std::cin >> input;
        }
    }

    if (turnEnded)
    {
        player->updateCooldowns();
    }
}

void BattleState::placePlayerInCenter()
{
    levelManager->placePlayer(gameManager->getPlayer());
}

void BattleState::handleEnemyTurns()
{
    levelManager->removeDeadEnemies();

    for (std::shared_ptr<Enemy> enemy : levelManager->getEnemies())
    {
        if (enemy)
        {
            enemy->makeTurn();
        }
    }

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