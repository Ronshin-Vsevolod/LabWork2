/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include <gtest/gtest.h>

#include "Main.h"

int Runner(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}