#ifndef _MENU_HPP_
#define _MENU_HPP_

#include <conio.h>
#include <iostream>
#include <mingw.thread.h>
#include "offline_game.hpp"

/**************************************************************  

 * This header file states MENU class

***************************************************************/

class Menu
{
private:
    // game mode: offline or online
    const int OFFLINE = 1;
    const int ONLINE = 2;

    // that is what you think it is
    Game* game;

    // score of last finished game
    int score;

public:
    Menu();

    void action();

    char print_guide();

    void print_result();

    ~Menu();
};

#endif