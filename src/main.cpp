#include "menu.hpp"
#include <conio.h>

int main()
{
    Menu* menu = new Menu();

    menu->action();

    delete menu;

    return 0;
}