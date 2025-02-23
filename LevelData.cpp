#include "LevelData.h"

LevelData::LevelData(int levelNumber, int rewardMoney, int rewardSkulls, int fieldSize, const std::vector<std::vector<std::string>>& waves)
    : levelNumber(levelNumber), rewardMoney(rewardMoney), rewardSkulls(rewardSkulls), fieldSize(fieldSize), waves(waves) {}