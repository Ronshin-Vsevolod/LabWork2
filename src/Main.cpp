/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "GameManager.h"
#include "MainMenuState.h"

int main(int argc, char **argv)
{
    GameManager gameManager;

    bool hasSave = gameManager.loadGame();

    MainMenuState mainMenuState(&gameManager, hasSave);
    mainMenuState.enter();

    std::cout << "Game finished. Goodbye!\n";
    return 0;
}
