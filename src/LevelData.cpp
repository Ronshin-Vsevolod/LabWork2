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
            WaveInfo("Первая волна", 0,
            {
                "Ashigaru", "Ashigaru", "Twin Tachi"
            }),
            WaveInfo("Вторая волна", 5,
            {
                "Ashigaru", "Twin Tachi", "Yari Master"
            }),
            WaveInfo("Третья волна", 10,
            {
                "Ashigaru", "Twin Tachi", "Yari Master", "Tetsudo Bandit"
            })
        }
    ),
    LevelData
    (
        2, 7, 2, 5,
        {
            WaveInfo("Первая волна", 0,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit"
            }),
            WaveInfo("Вторая волна", 6,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant"
            }),
            WaveInfo("Третья волна", 12,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant"
            })
        }
    ),
    LevelData
    (
        3, 8, 3, 7,
        {
            WaveInfo("Первая волна", 0,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant", "Ashigaru"}),
            WaveInfo("Вторая волна", 7,
            {
                "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Yari Master"}),
            WaveInfo("Третья волна", 14,
            {
                "Armored Giant", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Ashigaru"})
            }
    ),
    LevelData
    (
        4, 9, 4, 7,
        {
            WaveInfo("Первая волна", 0,
            {
                "Armored Giant", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Ashigaru"}),
            WaveInfo("Вторая волна", 8,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant", "Ashigaru"}),
            WaveInfo("Третья волна", 16,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Ashigaru", "Twin Tachi"})
            }
    ),
    LevelData
    (
        5, 10, 5, 9,
        {
            WaveInfo("Первая волна", 0,
            {
                "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Yari Master", "Ashigaru", "Twin Tachi"}),
            WaveInfo("Вторая волна", 9,
            {
                "Armored Giant", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Ashigaru", "Twin Tachi"}),
            WaveInfo("Третья волна", 18,
            {
                "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant", "Ashigaru", "Twin Tachi", "Yari Master"}),
            WaveInfo("Четвертая волна", 27,
            {
                "Yari Master", "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Ashigaru", "Twin Tachi", "Yari Master", "Tetsudo Bandit"}),
            WaveInfo("Пятая волна", 36,
            {
                "Tetsudo Bandit", "Armored Giant", "Twin Tachi", "Yari Master", "Ashigaru", "Twin Tachi", "Yari Master", "Tetsudo Bandit", "Armored Giant"})
            }
    )
};
