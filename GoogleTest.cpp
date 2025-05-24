/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include <gtest/gtest.h>
#include "include/Player.h"
#include "include/LevelManager.h"
#include "include/LevelData.h"
#include "include/GameManager.h"
#include "include/Skills.h"
#include "include/EnemyFactory.h"
#include "include/Effect.h"
#include "include/BattleState.h"
#include "include/PurchaseState.h"
#include "include/SkullShopState.h"
#include "include/LoadGameState.h"
#include "include/MainMenuState.h"
#include <fstream>
#include <typeinfo>

std::pair<std::shared_ptr<Player>, std::shared_ptr<Enemy>> setupCombatTest()
{
    static PlayerData playerData;
    playerData.health = 10;
    playerData.maxHP = 10;

    auto player = std::make_shared<Player>(&playerData);
    auto enemy = std::make_shared<Enemy>("Test Enemy", 10, 10, 2, false);
    return
    {player, enemy};
}

class StartTurnDamageEffect : public Effect
{
public:
    StartTurnDamageEffect(int value, int duration)
        : Effect("Start Turn Damage", duration), value(value){}

    TriggerType getTriggerType() const override
    {
        return TriggerType::ON_TURN_START;
    }

    void apply(Personage* target) override
    {
        if (target)
        {
            target->health -= value;
            std::cout << target->name << " получает " << value << " урона от эффекта " << name << "!\n";
        }
    }

private:
    int value;
};

class OnDamageEffect : public Effect
{
public:
    OnDamageEffect(int value, int duration)
        : Effect("On Damage", duration), value(value){}

    TriggerType getTriggerType() const override
    {
        return TriggerType::ON_DAMAGE;
    }

    void apply(Personage* target) override
    {
        if (target)
        {
            target->health -= value;
            std::cout << target->name << " получает дополнительно" << value << " урона от эффекта " << name << "!\n";
        }
    }

private:
    int value;
};

TEST(CombatSystemTest, impactOfSkills)
{
    auto executor = std::make_shared<Enemy>("Executor", 10, 10, 2, true);
    auto assistant = std::make_shared<Enemy>("Assistant", 10, 10, 3, false);

    auto yari = std::make_shared<ClassicSkill>("Yari", 2, 2, std::vector<int>{2}, 0, 0);
    auto tanto = std::make_shared<ClassicSkill>("Tanto", 1, 1, std::vector<int>{1}, 0, 0);
    auto totem = std::make_shared<HealingTotemSkill>();

    executor->skills.push_back(yari);
    executor->skills.push_back(tanto);
    executor->skills.push_back(totem);

    executor->prepareSkill(0);
    executor->prepareSkill(1);
    EXPECT_EQ(executor->prepareStack.size(), 2);


    executor->prepareSkill(0);
    EXPECT_EQ(executor->prepareStack.size(), 2);

    std::vector<Personage*> targets = {assistant.get(), executor.get()};

    executor->useSkills(5, targets);
    EXPECT_EQ(assistant->health, 9);

    executor->prepareSkill(2);
    executor->useSkills(5, targets);

    EXPECT_EQ(assistant->health, 10);
}

TEST(CombatSystemTest, Effects)
{
    auto player = std::make_shared<Player>(nullptr);

    auto startTurnDamage = std::make_shared<StartTurnDamageEffect>(2, 2);
    auto onDamage = std::make_shared<OnDamageEffect>(1, 1);
    player->addEffect(startTurnDamage);
    player->addEffect(onDamage);

    EXPECT_EQ(player->health, 6);
    EXPECT_EQ(startTurnDamage->duration, 2);
    EXPECT_EQ(onDamage->duration, 1);

    player->triggerEffect(static_cast<int>(Effect::TriggerType::ON_TURN_START));
    EXPECT_EQ(player->health, 4);
    EXPECT_EQ(startTurnDamage->duration, 2);

    player->takeDamage(0);
    EXPECT_EQ(player->health, 3);
    EXPECT_EQ(onDamage->duration, 1);

    player->decreaseEffectsDuration();
    EXPECT_EQ(startTurnDamage->duration, 1);
    EXPECT_EQ(player->effects.size(), 1);
}

TEST(EnemyAITest, BasicBehavior)
{

    int fieldSize = 5;
    EnemyAI ai(fieldSize);
    auto player = std::make_shared<Player>(nullptr);
    player->location = 4;

    auto enemy = std::make_shared<Enemy>("Враг", 10, 10, 2, true);
    enemy->location = 0;
    auto skill = std::make_shared<ClassicSkill>("TestSkill", 1, 1, std::vector<int>{1}, 0, 0);
    enemy->prepareStack.push_back(skill);

    LevelData levelData(1, 100, 10, fieldSize,{});
    LevelManager lm(&levelData);

    lm.updateOccupiedCells(*player);

    lm.getEnemies().push_back(enemy);
    lm.updateOccupiedCells(*player);

    ai.makeTurn(enemy.get(), *player, lm);
    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(enemy->location, 1);

    auto blockingEnemy = std::make_shared<Enemy>("Блокирующий", 10, 10, 2, true);
    blockingEnemy->location = 2;
    lm.getEnemies().push_back(blockingEnemy);
    lm.updateOccupiedCells(*player);

    enemy->path = 0;

    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(enemy->location, 1);

    auto blockingEnemyLeft = std::make_shared<Enemy>("Блокирующий слева", 10, 10, 2, true);
    blockingEnemyLeft->location = 0;
    lm.getEnemies().push_back(blockingEnemyLeft);
    lm.updateOccupiedCells(*player);

    enemy->path = 0;

    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(enemy->location, 1);
    EXPECT_EQ(enemy->path, 0);
}

TEST(EnemyAITest, MilitaryBehavior)
{
    int fieldSize = 5;
    EnemyAI ai(fieldSize);
    auto player = std::make_shared<Player>(nullptr);
    player->location = 2;

    auto enemy = std::make_shared<Enemy>("Buka", 10, 10, 2, true);
    enemy->location = 0;

    auto rangedSkill = std::make_shared<ClassicSkill>("Дальний удар", 2, 2, std::vector<int>{2}, 0, 0);

    auto meleeSkill = std::make_shared<ClassicSkill>("Ближний удар", 1, 1, std::vector<int>{1}, 0, 0);

    enemy->skills.push_back(meleeSkill);
    enemy->skills.push_back(rangedSkill);
    enemy->skills.push_back(meleeSkill);

    LevelData levelData(1, 100, 10, fieldSize,{});
    LevelManager lm(&levelData);

    lm.updateOccupiedCells(*player);
    lm.getEnemies().push_back(enemy);
    lm.updateOccupiedCells(*player);

    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(enemy->path, 2);

    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(enemy->prepareStack.size(), 1);
    EXPECT_EQ(enemy->prepareStack[0]->name, "Дальний удар");

    int playerHealthBefore = player->health;
    ai.makeTurn(enemy.get(), *player, lm);
    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(player->health, playerHealthBefore - 2);

    EXPECT_EQ(enemy->prepareStack.size(), 0);

    player->health = 10;

    enemy->skills.clear();
    enemy->prepareStack.push_back(meleeSkill);
    enemy->location = 0;

    enemy->path = 0;
    ai.makeTurn(enemy.get(), *player, lm);
    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(enemy->location, 1);

    playerHealthBefore = player->health;
    ai.makeTurn(enemy.get(), *player, lm);

    EXPECT_EQ(player->health, playerHealthBefore - 1);

    EXPECT_EQ(enemy->prepareStack.size(), 0);
}


TEST(LevelSystemTest, LevelSystem)
{
        std::vector<WaveInfo> testWaves = {
        WaveInfo("Волна 1", 0, {"Ashigaru", "Twin Tachi"}),
        WaveInfo("Волна 2", 5, {"Ashigaru", "Twin Tachi", "Yari Master"})
        };
    LevelData testLevel(1, 50, 10, 5, testWaves);

    LevelManager lm(&testLevel);

    auto occupiedCells = lm.getOccupiedCells();
    EXPECT_EQ(occupiedCells.size(), testLevel.fieldSize);

    auto player = std::make_shared<Player>(nullptr);
        player->location = 0;

    lm.placePlayer(player.get());

    EXPECT_TRUE(lm.getOccupiedCells()[player->location]);
    EXPECT_FALSE(lm.getOccupiedCells()[0]);

    EXPECT_EQ(lm.getEnemies().size(), 0);
    lm.trySpawnEnemies(0, *player);
    EXPECT_EQ(lm.getEnemies().size(), 2);

    EXPECT_FALSE(lm.areAllEnemiesDefeated());

    for (auto& enemy : lm.getEnemies())
    {
        enemy->health = 0;
    }
    lm.removeDeadEnemies(*player);

    EXPECT_TRUE(lm.areAllEnemiesDefeated());

    lm.trySpawnEnemies(0, *player);
    for (auto& enemy : lm.getEnemies())
    {
        enemy->health = 0;
    }
    lm.removeDeadEnemies(*player);

    EXPECT_TRUE(lm.areAllEnemiesDefeated());

    PlayerData testPlayerData;
    testPlayerData.money = 100;
    testPlayerData.skulls = 5;

    testPlayerData.money += testLevel.rewardMoney;
    testPlayerData.skulls += testLevel.rewardSkulls;


    EXPECT_EQ(testPlayerData.money, 150);
    EXPECT_EQ(testPlayerData.skulls, 15);
}

TEST(LevelSystemTest, EnemyWaves)
{
        std::vector<WaveInfo> testWaves = {
        WaveInfo("Первая волна", 0,
        {"Ashigaru", "Twin Tachi"}),
        WaveInfo("Вторая волна", 5,{"Yari Master"})};
    LevelData testLevel(1, 50, 10, 5, testWaves);

    LevelManager lm(&testLevel);

    auto player = std::make_shared<Player>(nullptr);
    player->location = 0;

    lm.placePlayer(player.get());

    lm.trySpawnEnemies(0, *player);
        lm.trySpawnEnemies(0, *player);

    EXPECT_EQ(lm.getEnemies().size(), 2);

    bool hasAshigaru = false;
    bool hasTwinTachi = false;

    for (const auto& enemy : lm.getEnemies())
    {
        if (enemy->name == "Ashigaru") hasAshigaru = true;
        if (enemy->name == "Twin Tachi") hasTwinTachi = true;
    }
    EXPECT_TRUE(hasAshigaru);
    EXPECT_TRUE(hasTwinTachi);

    for (const auto& enemy : lm.getEnemies())
    {
        EXPECT_NE(enemy->location, player->location);
        EXPECT_GE(enemy->location, 0);
        EXPECT_LT(enemy->location, testLevel.fieldSize);
    }

    int initialEnemyCount = lm.getEnemies().size();

    lm.trySpawnEnemies(3, *player);
    lm.trySpawnEnemies(3, *player);

    EXPECT_EQ(lm.getEnemies().size(), initialEnemyCount);

    lm.trySpawnEnemies(5, *player);
    lm.trySpawnEnemies(5, *player);

    int maxPossibleEnemies = static_cast<int>(testLevel.fieldSize * 3 / 4);
    EXPECT_EQ(lm.getEnemies().size(), maxPossibleEnemies);
}



TEST(ShopSystem, PurchaseShop)
{
    GameManager gm;
    PlayerData* playerData = gm.getPlayerData();
    playerData->money = 100;
    playerData->health = 10;
    playerData->maxHP = 10;

    playerData->unlockedSkills.insert("Katana");
    playerData->unlockedSkills.insert("Tanto");

    auto initialSkill = SkillFactory::create("Katana");

    ClassicSkill* katana = dynamic_cast<ClassicSkill*>(initialSkill.get());
    EXPECT_TRUE(katana != nullptr);

    int initialDamage = katana->damage;

    playerData->skills.push_back(initialSkill);

    gm.updatePlayer();

    PurchaseState purchaseState(&gm);

    int startingMaxHP = playerData->maxHP;
    int startingMoney = playerData->money;

    purchaseState.handleInput("increase_max_hp");

    EXPECT_EQ(playerData->maxHP, startingMaxHP + 1);
    EXPECT_LT(playerData->money, startingMoney);

    startingMoney = playerData->money;

    purchaseState.handleInput("upgrade_skill");

    purchaseState.handleInput("1");

    EXPECT_GT(katana->damage, initialDamage);
    EXPECT_LT(playerData->money, startingMoney);

    playerData->health = 5;
    gm.updatePlayer();

    startingMoney = playerData->money;

    purchaseState.handleInput("increase_hp");

    EXPECT_EQ(gm.getPlayer().health, playerData->maxHP);
    EXPECT_LT(playerData->money, startingMoney);

    startingMoney = playerData->money;

    purchaseState.handleInput("buy_skill");

    purchaseState.handleInput("1");

    EXPECT_EQ(playerData->skills.size(), 2);
    EXPECT_LT(playerData->money, startingMoney);

    startingMoney = playerData->money;

    purchaseState.handleInput("buy_skill");
    purchaseState.handleInput("1");

    EXPECT_EQ(playerData->skills.size(), 2);
    EXPECT_EQ(playerData->money, startingMoney);

    purchaseState.handleInput("exit");
    purchaseState.handleInput("back");
}

TEST(ShopSystem, skullShop)
{
    GameManager gm;
    PlayerData* playerData = gm.getPlayerData();

    playerData->skulls = 50;

    SkullShopState* skullShopState = new SkullShopState(&gm);

    skullShopState->handleInput("show_skills");

    int startingSkulls = playerData->skulls;
    size_t initialUnlockedCount = playerData->unlockedSkills.size();

    skullShopState->handleInput("unlock");

    auto skillToUnlock = SkillFactory::create("Katana");
    int skillCost = skillToUnlock->skullCost;

    skullShopState->handleInput("Katana");

    EXPECT_EQ(playerData->unlockedSkills.size(), initialUnlockedCount + 1);
    EXPECT_TRUE(playerData->isSkillUnlocked("Katana"));
    EXPECT_EQ(playerData->skulls, startingSkulls - skillCost);

    skullShopState->handleInput("back");

    skullShopState->handleInput("exit");

    delete skullShopState;
}


TEST(GameManagerTest, GameFlow)
{
    GameManager gm;

    EXPECT_EQ(gm.getCurrentState(), nullptr);

    gm.changeState(new MainMenuState(&gm));
    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(MainMenuState));

    gm.openSkullShop();
    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(SkullShopState));

    gm.changeState(new MainMenuState(&gm));
    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(MainMenuState));

    gm.startNewGame();
    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(BattleState));
    EXPECT_EQ(gm.getCurrentLevelIndex(), 0);

    gm.openPurchaseState();
    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(PurchaseState));

    gm.changeState(new BattleState(&gm));
    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(BattleState));

    int currentLevelIndex = gm.getCurrentLevelIndex();

    gm.completeCurrentLevel();

    EXPECT_EQ(gm.getCurrentLevelIndex(), currentLevelIndex + 1);

    EXPECT_EQ(typeid(*gm.getCurrentState()), typeid(PurchaseState));

    GameManager newGm;
    newGm.continueGame();
    EXPECT_EQ(typeid(*newGm.getCurrentState()), typeid(LoadGameState));

    gm.saveGame();
}


TEST(SaveLoadCycle, SaveLoad)
{
    GameManager gm;
    gm.getPlayer().playerData->money = 1000;
    gm.getPlayer().playerData->skulls = 50;
    gm.getPlayer().playerData->health = 15;
    gm.getPlayer().playerData->maxHP = 20;
    gm.getPlayer().playerData->currentLevel = 3;

    auto testSkill = std::make_shared<ClassicSkill>("Test skill", 2, 2, std::vector<int>{2}, 0, 0);
    gm.getPlayer().skills.push_back(testSkill);

    gm.saveGame();

    gm.getPlayer().playerData->money = 500;
    gm.getPlayer().playerData->skulls = 10;
    gm.getPlayer().playerData->health = 5;
    gm.getPlayer().playerData->maxHP = 5;
    gm.getPlayer().playerData->currentLevel = 5;
    gm.getPlayer().skills.clear();

    bool loadSuccess = gm.loadGame();
    EXPECT_TRUE(loadSuccess);

    EXPECT_EQ(gm.getPlayer().playerData->money, 1000);
    EXPECT_EQ(gm.getPlayer().playerData->skulls, 50);
    EXPECT_EQ(gm.getPlayer().playerData->health, 15);
    EXPECT_EQ(gm.getPlayer().playerData->maxHP, 20);
    EXPECT_EQ(gm.getPlayer().playerData->currentLevel, 3);
    EXPECT_EQ(gm.getCurrentLevelIndex(), 3);

    EXPECT_EQ(gm.getPlayer().skills.size(), 1);
    EXPECT_EQ(gm.getPlayer().skills[0]->name, "Test skill");

    std::remove("SaveFile");
}

TEST(SaveLoadCycle, WithoutSave)
{
    std::remove("SaveFile");

    GameManager gm;
    bool loadSuccess = gm.loadGame();

    EXPECT_FALSE(loadSuccess);

    EXPECT_EQ(gm.getCurrentLevelIndex(), 0);
    EXPECT_EQ(gm.getPlayer().playerData->currentLevel, 0);
    EXPECT_EQ(gm.getPlayer().playerData->money, 0);
    EXPECT_EQ(gm.getPlayer().playerData->skulls, 0);
    EXPECT_EQ(gm.getPlayer().playerData->health, 6);
    EXPECT_EQ(gm.getPlayer().playerData->maxHP, 6);

    gm.getPlayer().playerData->money = 1500;
    gm.getPlayer().playerData->currentLevel = 2;
    gm.saveGame();

    GameManager newGm;
    loadSuccess = newGm.loadGame();

    EXPECT_TRUE(loadSuccess);

    EXPECT_EQ(newGm.getPlayer().playerData->money, 1500);
    EXPECT_EQ(newGm.getPlayer().playerData->currentLevel, 2);
    EXPECT_EQ(newGm.getCurrentLevelIndex(), 2);
    std::remove("SaveFile");
}

int Runner(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
