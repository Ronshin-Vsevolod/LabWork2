#include "PurchaseState.h"

PurchaseState::PurchaseState(GameManager* gameManager)
    : gameManager(gameManager), player(gameManager->getPlayer())
{
}

void PurchaseState::enter()
{
    std::cout << "Вход в этап покупок.\n";
    std::cout << "1. Улучшить навык\n";
    std::cout << "2. Увеличить HP\n";
    std::cout << "3. Увеличить максимум HP\n";
    std::cout << "4. Вернуться в главное меню\n";
}

void PurchaseState::exit()
{
    std::cout << "Выход из этапа покупок.\n";
}

void PurchaseState::handleInput(const std::string& inputData)
{
    if (inputData == "1")
    {
        upgradeSkill();
    }
    else if (inputData == "2")
    {
        increaseHP();
    }
    else if (inputData == "3")
    {
        increaseMaxHP();
    }
    else if (inputData == "4")
    {
        std::cout << "Возврат в главное меню.\n";
        gameManager->changeState(new MainMenuState(gameManager));
    }
    else
    {
        std::cout << "Неизвестная команда.\n";
    }
}

void PurchaseState::upgradeSkill()
{
    std::cout << "Выберите навык для улучшения:\n";
    for (const std::shared_ptr<Skill>& skill : player->playerData->skills)
    {
        std::unordered_map<std::string, SkillInfo>::iterator it = SkillRegistry.find(skill->name);
        if (it != SkillRegistry.end())
        {
            std::cout << skill->name << " - " << it->second.coinCost << " монет\n";
        }
    }

    std::string skillName;
    std::cin >> skillName;

    std::unordered_map<std::string, SkillInfo>::iterator it = SkillRegistry.find(skillName);
    if (it != SkillRegistry.end())
    {
        if (upgradeSkill(it->second.skill, player->playerData->money))
        {
            std::cout << "Навык " << skillName << " улучшен!\n";
        }
        else
        {
            std::cout << "Не удалось улучшить навык.\n";
        }
    }
    else
    {
        std::cout << "Неизвестный навык.\n";
    }
}

void PurchaseState::increaseHP()
{
    if (player->playerData->money >= 15)
    {
        player->playerData->money -= 15;
        player->playerData->health += 10;
        std::cout << "HP увеличен!\n";
    }
    else
    {
        std::cout << "Недостаточно денег для увеличения HP.\n";
    }
}

void PurchaseState::increaseMaxHP()
{
    if (player->playerData->money >= 25)
    {
        player->playerData->money -= 25;
        player->playerData->maxHP += 20;
        std::cout << "Максимум HP увеличен!\n";
    }
    else
    {
        std::cout << "Недостаточно денег для увеличения максимума HP.\n";
    }
}

bool PurchaseState::upgradeSkill(std::shared_ptr<Skill> skill, int& money)
{
    auto it = SkillRegistry.find(skill->name);
    if (it != SkillRegistry.end() && money >= it->second.coinCost)
    {
        money -= it->second.coinCost;
        skill->damage += 1;
        std::cout << "Навык " << skill->name << " улучшен! Новый урон: " << skill->damage << "\n";
        return true;
    }
    return false;
}