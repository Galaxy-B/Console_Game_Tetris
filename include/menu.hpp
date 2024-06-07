#ifndef _MENU_HPP_
#define _MENU_HPP_

#include <conio.h>
#include <iostream>
#include <mingw.thread.h>
#include "game.hpp"

/**************************************************************  

 * This header file states MENU class

***************************************************************/

class Menu
{
private:
    // that is what you think it is
    Game* game;

    // score of last finished game
    int score;

public:
    Menu();

    void action();

    void print_guide();

    void print_result();

    ~Menu();
};

#endif