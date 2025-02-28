#include "SkullShopState.h"
#include "MainMenuState.h"

SkullShopState::SkullShopState(GameManager* gameManager)
    : gameManager(gameManager), player(gameManager->getPlayer())
{
}

void SkullShopState::enter()
{
    std::cout << "Вход в магазин черепов.\n";
    std::cout << "Доступные навыки:\n";
    for (const std::pair<const std::string, std::shared_ptr<Skill>>& entry : SkillRegistry)
    {
        std::cout << entry.first << " - " << entry.second->skullCost << " черепов\n";
    }
    
    handleInput("");
}

void SkullShopState::exit()
{
    std::cout << "Выход из магазина черепов.\n";
}

void SkullShopState::handleInput(const std::string& inputData)
{
    while (true)
    {
        std::string inputData;
        std::cin >> inputData;

        if (inputData == "back")
        {
            std::cout << "Возврат в главное меню.\n";
            gameManager->changeState(new MainMenuState(gameManager));
            break;
        }

        std::unordered_map<std::string, std::shared_ptr<Skill>>::iterator it = SkillRegistry.find(inputData);
        if (it != SkillRegistry.end())
        {
            if (player->playerData->skulls >= it->second->skullCost)
            {
                player->playerData->skulls -= it->second->skullCost;
                player->playerData->skills.push_back(it->second);
                std::cout << "Навык " << it->second->name << " разблокирован!\n";
            }
            else
            {
                std::cout << "Недостаточно черепов. У вас " << player->playerData->skulls << " черепов, а нужно " << it->second->skullCost << ".\n";
            }
        }
        else
        {
            std::cout << "Неизвестный навык.\n";
        }
    }
}