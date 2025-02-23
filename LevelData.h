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

#endif