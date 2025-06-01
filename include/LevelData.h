/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include <vector>
#include <string>

struct WaveInfo
{
    std::string name;
    int startTurn;
    std::vector<std::string> enemies;

    WaveInfo(const std::string& name,
    int start,
    const std::vector<std::string>& enemies)
    : name(name),
    startTurn(start),
    enemies(enemies){}
};

class LevelData
{
    public:
    LevelData(int levelNumber, int rewardMoney, int rewardSkulls, int fieldSize, const std::vector<WaveInfo>& waves)
    : levelNumber(levelNumber), rewardMoney(rewardMoney), rewardSkulls(rewardSkulls), fieldSize(fieldSize), waves(waves)
        {}
        int levelNumber;
        int rewardMoney;
        int rewardSkulls;
        int fieldSize;
        std::vector<WaveInfo> waves;
};

extern std::vector<LevelData> Levels;

#endif
