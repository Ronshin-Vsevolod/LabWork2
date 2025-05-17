/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "BattleState.h"
#include "GameManager.h"
#include "LevelManager.h"
#include "MainMenuState.h"
#include "EnemyFactory.h"
#include "Player.h"
#include <iostream>
#include <sstream>
#include <optional>

const std::vector<CommandInfo> BattleState::availableCommands =
{
    {
        "attack", "Атаковать противника", 1
    },
    {
        "move_left", "Движение влево", 2
    },
    {
        "move_right", "Движение вправо", 3
    },
    {
        "prepare_skill", "Подготовить навык", 4
    },
    {
        "swap_with_enemy", "Поменяться местами с противником", 5
    },
    {
        "turn_around", "Повернуться", 6
    },
    {
        "back", "Вернуться в главное меню", 7
    },
    {
        "end_turn", "Завершить ход", 8
    }
};

BattleState::BattleState(GameManager* gameManager)
    : gameManager(gameManager),
      currentTurn(0),
      isPlayerTurn(true)
{
    levelManager = std::make_unique<LevelManager>(gameManager->getCurrentLevelData());
}

void BattleState::enter()
{
    std::cout << "Вход в этап битвы.\n";
    std::cout << "Текущий уровень: " << levelManager->getLevelData()->levelNumber << "\n";
    std::cout << "Позиция игрока: " << gameManager->getPlayer().location << "\n";

    levelManager->placePlayer(&gameManager->getPlayer());
    levelManager->trySpawnEnemies(currentTurn, gameManager->getPlayer());
    isPlayerTurn = true;

    bool shouldExit = false;
    std::string inputData;

    while (!shouldExit)
    {
        if (isPlayerTurn)
        {
            if (currentTurn > 0)
            {
                    gameManager->getPlayer().triggerEffect(2);

                if (gameManager->getPlayer().health <= 0)
                {
                    std::cout << "Вы проиграли!\n";
                    gameManager->changeState(new MainMenuState(gameManager));
                    shouldExit = true;
                    continue;
                }
            }

            std::cout << "Ход игрока.\n";
            printAvailableCommands();
            std::getline(std::cin, inputData);
            shouldExit = handleInput(inputData);

            if (!isPlayerTurn && !shouldExit)
            {
                levelManager->removeDeadEnemies(gameManager->getPlayer());

                for (auto& enemy : levelManager->getEnemies())
                {
                        if (enemy) enemy->triggerEffect(2);
                }
            }
        }
        else
        {
            levelManager->trySpawnEnemies(currentTurn, gameManager->getPlayer());
            std::cout << "Ход противников.\n";
            handleEnemyTurns();
            isPlayerTurn = true;

            gameManager->getPlayer().triggerEffect(3);
            gameManager->getPlayer().decreaseEffectsDuration();
            Player& player = gameManager->getPlayer();

            if (player.health <= 0)
            {
                std::cout << "Вы проиграли!\n";
                gameManager->changeState(new MainMenuState(gameManager));
                shouldExit = true;
            }
            else
            {
                levelManager->trySpawnEnemies(currentTurn, gameManager->getPlayer());

                if (levelManager->areAllEnemiesDefeated())
                {
                    std::cout << "Все противники повержены! Уровень пройден!\n";
                    player.playerData->skulls += levelManager->getLevelData()->rewardSkulls;
                    std::cout << "Получено черепов: " << levelManager->getLevelData()->rewardSkulls << "\n";


                    player.playerData->health = player.health;

                    player.playerData->skills = player.skills;

                    gameManager->completeCurrentLevel();
                    shouldExit = true;
                }
            }
        }
    }
}

void BattleState::exit()
{
    std::cout << "Выход из этапа битвы.\n";
}

bool BattleState::handleInput(const std::string& inputData)
{
    Player& player = gameManager->getPlayer();
    PlayerCommand command = parseCommand(inputData);

    switch (command)
    {
        case PlayerCommand::ATTACK:
        {
            std::vector<Personage*> targets;
            for (const std::shared_ptr<Enemy>& enemy : levelManager->getEnemies())
            {
                targets.push_back(enemy.get());
            }

            if (!player.prepareStack.empty())
            {
                Skill* skill = player.prepareStack[0].get();
                std::cout << "Использование навыка: " << skill->name << "\n";

                player.useSkills(levelManager->getLevelData()->fieldSize, targets);
                levelManager->removeDeadEnemies(gameManager->getPlayer());

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "У вас нет подготовленных навыков для атаки! Сначала подготовьте навык.\n";
                return false;
            }
        }

        case PlayerCommand::MOVE_LEFT:
        {
            const std::vector<bool>& occupied = levelManager->getOccupiedCells();
            int position = player.location;

            if (position > 0 && !occupied[position - 1])
            {
                std::cout << "Движение влево.\n";
                player.moveLeft(levelManager->getLevelData()->fieldSize, occupied);
                levelManager->updateOccupiedCells(player);

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "Движение невозможно!\n";
                return false;
            }
        }

        case PlayerCommand::MOVE_RIGHT:
        {
            const std::vector<bool>& occupied = levelManager->getOccupiedCells();
            int position = player.location;
            int fieldSize = levelManager->getLevelData()->fieldSize;

            if (position < fieldSize - 1 && !occupied[position + 1])
            {
                std::cout << "Движение вправо.\n";
                player.moveRight(fieldSize, occupied);
                levelManager->updateOccupiedCells(player);

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "Движение невозможно!\n";
                return false;
            }
        }

        case PlayerCommand::PREPARE_SKILL:
        {
            const std::vector<std::shared_ptr<Skill>>& skills = player.skills;

            if (skills.empty())
            {
                std::cout << "У вас нет доступных навыков!\n";
                return false;
            }

            std::cout << "Доступные навыки:\n";
            for (size_t i = 0; i < skills.size(); ++i)
            {
                ClassicSkill* classicSkill = dynamic_cast<ClassicSkill*>(skills[i].get());

                std::cout << i + 1 << ". " << skills[i]->name;

                if (classicSkill)
                {
                    std::cout << " (урон: " << classicSkill->damage << ")";
                }

                std::cout << " (кулдаун: " << skills[i]->cooldownTimer << "/" << skills[i]->cooldown << ")";
                std::cout << std::endl;
            }

            std::cout << "Введите индекс навыка: ";
            std::string indexStr;
            std::getline(std::cin, indexStr);

            bool isValidNumber = true;
            if (indexStr.empty()) isValidNumber = false;

            for (char c : indexStr)
            {
                if (!std::isdigit(c))
                {
                    isValidNumber = false;
                    break;
                }
            }

            if (!isValidNumber)
            {
                std::cout << "Ошибка: требуется числовой индекс!\n";
                return false;
            }

                size_t index = std::stoul(indexStr) - 1;

            if (index < skills.size())
            {

                if (skills[index]->cooldownTimer > 0)
                {
                    std::cout << "Навык '" << skills[index]->name << "' на перезарядке (осталось: "
                              << skills[index]->cooldownTimer << " ходов)!\n";
                    return false;
                }

                bool alreadyPrepared = false;
                for (const auto& skill : player.prepareStack)
                {
                    if (skill->name == skills[index]->name)
                    {
                        alreadyPrepared = true;
                        break;
                    }
                }

                if (alreadyPrepared)
                {
                    std::cout << "Навык '" << skills[index]->name << "' уже подготовлен!\n";
                    return false;
                }

                player.prepareSkill(index);
                std::cout << "Навык '" << skills[index]->name << "' подготовлен.\n";

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "Неверный индекс навыка!\n";
                return false;
            }
        }

        case PlayerCommand::SWAP_WITH_ENEMY:
        {
            std::vector<std::shared_ptr<Enemy>>& enemies = levelManager->getEnemies();
            player.swapWithEnemy(enemies);

            std::cout << "Поменялись местами с противником.\n";
            levelManager->updateOccupiedCells(player);

            player.updateCooldowns();
            isPlayerTurn = false;
            return false;
        }

        case PlayerCommand::TURN_AROUND:
        {
            player.turnAround();
            std::cout << "Направление изменено.\n";


            player.updateCooldowns();
            isPlayerTurn = false;
            return false;
        }

        case PlayerCommand::BACK:
        {
            std::cout << "Возврат в главное меню.\n";

            gameManager->changeState(new MainMenuState(gameManager, true));
            return true;
        }

        case PlayerCommand::END_TURN:
        {
            std::cout << "Ход завершен.\n";


            player.updateCooldowns();
            isPlayerTurn = false;
            return false;
        }

        case PlayerCommand::UNKNOWN:
        default:
        {
            std::cout << "Неизвестная команда.\n";
            return false;
        }
    }
}

void BattleState::handleEnemyTurns()
{
    std::vector<std::shared_ptr<Enemy>>& enemies = levelManager->getEnemies();
    levelManager->removeDeadEnemies(gameManager->getPlayer());

    if (!enemies.empty())
    {
        std::cout << "Ход противников.\n";
    }
    else
    {
        std::cout << "Нет активных противников.\n";
    }

    for (std::shared_ptr<Enemy>& enemy : enemies)
    {
        if (enemy)
        {
            std::cout << enemy->name << " (здоровье: " << enemy->health << ", позиция: " << enemy->location << ") совершает ход...\n";
            enemy->makeTurn(gameManager->getPlayer(), *levelManager);
            if (gameManager->getPlayer().health <= 0)
            {
                break;
            }
            levelManager->removeDeadEnemies(gameManager->getPlayer());
        }
    }

    for (auto& enemy : enemies)
    {
        if (enemy)
        {
                enemy->triggerEffect(3);
            enemy->decreaseEffectsDuration();
        }
    }

    currentTurn++;
}

void BattleState::printAvailableCommands() const
{
    std::cout << "\nДоступные команды:\n";
    for (const CommandInfo& cmd : availableCommands)
    {
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nВведите номер команды или её название: ";
}

PlayerCommand BattleState::parseCommand(const std::string& input)
{
    int number = 0;
    bool isNumber = true;

    for (char c : input)
    {
        if (!std::isdigit(c))
        {
            isNumber = false;
            break;
        }
    }

    if (isNumber && !input.empty())
    {
        number = std::stoi(input);
        for (const CommandInfo& cmd : availableCommands)
        {
            if (cmd.number == number)
            {
                return static_cast<PlayerCommand>(number);
            }
        }
    }

    for (const CommandInfo& cmd : availableCommands)
    {
        if (cmd.name == input)
        {
            return static_cast<PlayerCommand>(cmd.number);
        }
    }

    return PlayerCommand::UNKNOWN;
}
