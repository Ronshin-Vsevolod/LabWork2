#include "SkullShopState.h"

SkullShopState::SkullShopState(GameManager* gameManager)
    : gameManager(gameManager), player(gameManager->getPlayer())
{
}

void SkullShopState::enter()
{
    std::cout << "Вход в магазин черепов.\n";
    std::cout << "Доступные навыки:\n";
    for (const std::pair<std::string, SkillInfo>& entry : SkillRegistry)
    {
        std::cout << entry.first << " - " << entry.second.skullCost << " черепов\n";
    }
    std::cout << "Введите название навыка для покупки или 'back' для возврата.\n";
}

void SkullShopState::exit()
{
    std::cout << "Выход из магазина черепов.\n";
}

void SkullShopState::handleInput(const std::string& inputData)
{
    if (inputData == "back")
    {
        std::cout << "Возврат в главное меню.\n";
        gameManager->changeState(new MainMenuState(gameManager));
    }
    else
    {
        auto it = SkillRegistry.find(inputData);
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
                std::cout << "Недостаточно черепов.\n";
            }
        }
        else
        {
            std::cout << "Неизвестный навык.\n";
        }
    }
}