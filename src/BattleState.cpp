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
        "attack", "Attack the enemy", 1
    },
    {
        "move_left", "Movement to the left", 2
    },
    {
        "move_right", "Movement to the right", 3
    },
    {
        "prepare_skill", "Prepare a skill", 4
    },
    {
        "swap_with_enemy", "Swap places with the enemy", 5
    },
    {
        "turn_around", "Turn around", 6
    },
    {
        "back", "Back to main menu", 7
    },
    {
        "end_turn", "Complete the move", 8
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
    std::cout << "Entering the battle phase.\n";
    std::cout << "Current level: " << levelManager->getLevelData()->levelNumber << "\n";
    std::cout << "Player position: " << gameManager->getPlayer().location << "\n";

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
                    std::cout << "You lost!\n";
                    gameManager->changeState(new MainMenuState(gameManager));
                    shouldExit = true;
                    continue;
                }
            }

            std::cout << "Player turn.\n";
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
            std::cout << "Enemy turn.\n";
            handleEnemyTurns();
            isPlayerTurn = true;

            gameManager->getPlayer().triggerEffect(3);
            gameManager->getPlayer().decreaseEffectsDuration();
            Player& player = gameManager->getPlayer();

            if (player.health <= 0)
            {
                std::cout << "You lost!\n";
                gameManager->changeState(new MainMenuState(gameManager));
                shouldExit = true;
            }
            else
            {
                levelManager->trySpawnEnemies(currentTurn, gameManager->getPlayer());

                if (levelManager->areAllEnemiesDefeated())
                {
                    std::cout << "All enemies defeated! Level passed!\n";
                    player.playerData->skulls += levelManager->getLevelData()->rewardSkulls;
                    std::cout << "Received skulls: " << levelManager->getLevelData()->rewardSkulls << "\n";


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
    std::cout << "Exiting the battle phase.\n";
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
                std::cout << "Using skill: " << skill->name << "\n";

                player.useSkills(levelManager->getLevelData()->fieldSize, targets);
                levelManager->removeDeadEnemies(gameManager->getPlayer());

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "You have no prepared skills for attack! Prepare a skill first.\n";
                return false;
            }
        }

        case PlayerCommand::MOVE_LEFT:
        {
            const std::vector<bool>& occupied = levelManager->getOccupiedCells();
            int position = player.location;

            if (position > 0 && !occupied[position - 1])
            {
                std::cout << "Movement to the left.\n";
                player.moveLeft(levelManager->getLevelData()->fieldSize, occupied);
                levelManager->updateOccupiedCells(player);

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "Movement is not possible!\n";
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
                std::cout << "Movement to the right.\n";
                player.moveRight(fieldSize, occupied);
                levelManager->updateOccupiedCells(player);

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "Movement is not possible!\n";
                return false;
            }
        }

        case PlayerCommand::PREPARE_SKILL:
        {
            const std::vector<std::shared_ptr<Skill>>& skills = player.skills;

            if (skills.empty())
            {
                std::cout << "You have no available skills!\n";
                return false;
            }

            std::cout << "Available skills:\n";
            for (size_t i = 0; i < skills.size(); ++i)
            {
                ClassicSkill* classicSkill = dynamic_cast<ClassicSkill*>(skills[i].get());

                std::cout << i + 1 << ". " << skills[i]->name;

                if (classicSkill)
                {
                    std::cout << " (damage: " << classicSkill->damage << ")";
                }

                std::cout << " (cooldown: " << skills[i]->cooldownTimer << "/" << skills[i]->cooldown << ")";
                std::cout << std::endl;
            }

            std::cout << "Enter the skill index: ";
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
                std::cout << "Error: a numerical index is required!\n";
                return false;
            }

                size_t index = std::stoul(indexStr) - 1;

            if (index < skills.size())
            {

                if (skills[index]->cooldownTimer > 0)
                {
                    std::cout << "Skill '" << skills[index]->name << "' is on cooldown (remaining: "
                              << skills[index]->cooldownTimer << " turns)!\n";
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
                    std::cout << "Skill '" << skills[index]->name << "' is already prepared!\n";
                    return false;
                }

                player.prepareSkill(index);
                std::cout << "Skill '" << skills[index]->name << "' prepared.\n";

                player.updateCooldowns();
                isPlayerTurn = false;
                return false;
            }
            else
            {
                std::cout << "Invalid skill index!\n";
                return false;
            }
        }

        case PlayerCommand::SWAP_WITH_ENEMY:
        {
            std::vector<std::shared_ptr<Enemy>>& enemies = levelManager->getEnemies();
            player.swapWithEnemy(enemies);

            std::cout << "Swapped places with the enemy.\n";
            levelManager->updateOccupiedCells(player);

            player.updateCooldowns();
            isPlayerTurn = false;
            return false;
        }

        case PlayerCommand::TURN_AROUND:
        {
            player.turnAround();
            std::cout << "Direction changed.\n";


            player.updateCooldowns();
            isPlayerTurn = false;
            return false;
        }

        case PlayerCommand::BACK:
        {
            std::cout << "Return to main menu.\n";

            gameManager->changeState(new MainMenuState(gameManager, true));
            return true;
        }

        case PlayerCommand::END_TURN:
        {
            std::cout << "Move completed.\n";


            player.updateCooldowns();
            isPlayerTurn = false;
            return false;
        }

        case PlayerCommand::UNKNOWN:
        default:
        {
            std::cout << "Unknown command.\n";
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
        std::cout << "Enemy turn.\n";
    }
    else
    {
        std::cout << "No active enemies.\n";
    }

    for (std::shared_ptr<Enemy>& enemy : enemies)
    {
        if (enemy)
        {
            std::cout << enemy->name << " (health: " << enemy->health << ", position: " << enemy->location << ") makes a move...\n";
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
    std::cout << "\nAvailable commands:\n";
    for (const CommandInfo& cmd : availableCommands)
    {
        std::cout << cmd.number << ". " << cmd.name << " - " << cmd.description << "\n";
    }
    std::cout << "\nEnter the command number or its name: ";
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
