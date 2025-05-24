/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "LevelData.h"

std::vector<LevelData> Levels =
{
    LevelData
    (
        1, 5, 1, 5,
        {
            WaveInfo("First wave", 0,
            {
                "Ashigaru", "Ashigaru", "Twin Tachi"
            }),
            WaveInfo("Second wave", 5,
            {
                "Ashigaru", "Twin Tachi", "Yari Master"
            }),
            WaveInfo("Third wave", 10,
            {
                "Ashigaru", "Twin Tachi", "Yari Master", "Tetsudo Bandit"
            })
        }
    ),
    LevelData
    (
        2, 7, 2, 5,
        {
            WaveInfo("First wave", 0,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit"
            }),
            WaveInfo("Second wave", 6,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant"
            }),
            WaveInfo("Third wave", 12,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant"
            })
        }
    ),
    LevelData
    (
        3, 8, 3, 7,
        {
            WaveInfo("First wave", 0,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant", "Ashigaru"}),
            WaveInfo("Second wave", 7,
            {
                "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Yari Master"}),
            WaveInfo("Third wave", 14,
            {
                "Armored Giant", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Ashigaru"})
            }
    ),
    LevelData
    (
        4, 9, 4, 7,
        {
            WaveInfo("First wave", 0,
            {
                "Armored Giant", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Ashigaru"}),
            WaveInfo("Second wave", 8,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant", "Ashigaru"}),
            WaveInfo("Third wave", 16,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Ashigaru", "Twin Tachi"})
            }
    ),
    LevelData
    (
        5, 10, 5, 9,
        {
            WaveInfo("First wave", 0,
            {
                "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Yari Master", "Ashigaru", "Twin Tachi"}),
            WaveInfo("Second wave", 9,
            {
                "Armored Giant", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Ashigaru", "Twin Tachi"}),
            WaveInfo("Third wave", 18,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant", "Ashigaru", "Twin Tachi", "Yari Master"}),
            WaveInfo("Fourth wave", 27,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Ashigaru", "Twin Tachi", "Yari Master", "Tetsudo Bandit"}),
            WaveInfo("Fifth wave", 36,
            {
                "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Yari Master", "Ashigaru", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant"})
            }
    )
};
