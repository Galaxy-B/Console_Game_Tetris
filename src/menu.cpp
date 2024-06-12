#include "menu.hpp"
#include "offline_game.hpp"
#include "online_game.hpp"
#include <cstdlib>
#include <iostream>

Menu::Menu() {}

void Menu::action()
{
    while (true)
    {        
        // print guides before user starts a new game
        char choice = print_guide(); 
        
        system("cls");

        // start a new game according to user's choice and get it to work
        game = (choice == '\r' ? new Online_Game(ONLINE) : new Game(OFFLINE));
        
        score = game->action();

        delete game;

        system("cls");

        // print result of the finished game
        print_result();
    }
}

char Menu::print_guide()
{
    std::cout << "\n\tWelcome to Tetris!\n";
    std::cout << "\n\tUse UP DOWN LEFT RIGHT keys to operate the tetromino!\n";
    std::cout << "\n\tPress [ENTER|any other key] to start a [ONLINE|OFFLINE] game:";

    return getch();
}

void Menu::print_result()
{
    std::cout << "\n\tOops! Game is over!\n";
    std::cout << "\n\tYour final score is: " << score << "\n";
    std::cout << "\n\tPress ENTER to start a new game:";

    while (getch() != '\r') {}
}

Menu::~Menu() {}