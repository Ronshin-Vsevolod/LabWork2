/* Ronshin Vsevolod st132572@student.spbu.ru
game core development: character and ability hierarchy based on class inheritance; simple AI; graphical interface
*/

#include "GameManager.h"
#include "MainMenuState.h"
#include <iostream>

int main(int argc, char **argv)
{
    GameManager gameManager;

    MainMenuState mainMenuState(&gameManager);
    mainMenuState.enter();

    std::cout << "Игра завершена. До свидания!\n";
    return 0;
}