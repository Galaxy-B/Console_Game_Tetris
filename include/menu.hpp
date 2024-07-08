#ifndef _MENU_HPP_
#define _MENU_HPP_

#include <conio.h>
#include <iostream>
#include <string>
#include <mingw.thread.h>

#include "offline_game.hpp"
#include "online_game.hpp"

/**************************************************************  

 * This header file states MENU class

***************************************************************/

class Menu
{
private:
    // that is what you think it is
    Game* game;

public:
    Menu();

    void action();

    char print_guide();

    void print_result(int score, int rival_score);

    ~Menu();
};

#endif