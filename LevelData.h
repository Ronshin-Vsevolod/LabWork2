#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include <vector>
#include <string>

class LevelData
{
public:
    LevelData(int levelNumber, int rewardMoney, int rewardSkulls, int fieldSize, const std::vector<std::vector<std::string>>& waves);

    int levelNumber;
    int rewardMoney;
    int rewardSkulls;
    int fieldSize;
    std::vector<std::vector<std::string>> waves;
};

static std::vector<LevelData> Levels =
{
    LevelData(1, 100, 10, 10, {
        {
            "Wave1", "3", "5", "Goblin", "Goblin", "Orc"
        },
        {
            "Wave2", "5", "7", "Orc", "Orc", "Goblin", "Goblin"
        }
    }),
    LevelData(2, 200, 20, 15, {
        {
            "Wave1", "5", "7", "Orc", "Orc", "Goblin", "Goblin"
        },
        {
            "Wave2", "7", "10", "Orc", "Orc", "Orc", "Goblin", "Goblin", "Goblin"
        },
        {
            "Wave3", "10", "12", "Orc", "Orc", "Orc", "Orc", "Goblin", "Goblin", "Goblin"
        }
    }),
    LevelData(3, 300, 30, 20, {
        {
            "Wave1", "7", "10", "Orc", "Orc", "Orc", "Goblin", "Goblin", "Goblin"
        },
        {
            "Wave2", "10", "12", "Orc", "Orc", "Orc", "Orc", "Goblin", "Goblin", "Goblin"
        },
        {
            "Wave3", "12", "15", "Orc", "Orc", "Orc", "Orc", "Orc", "Goblin", "Goblin", "Goblin"
        }
    }),
};

#endif