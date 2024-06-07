#include "menu.hpp"

Menu::Menu() {}

void Menu::action()
{
    // print guides before user starts a new game
    print_guide(); 

    while (true)
    {        
        system("cls");

        // start a new game and get it to work
        Game* game = new Game();

        score = game->action(); 

        delete game;

        system("cls");

        // print result of the finished game
        print_result();
    }
}

void Menu::print_guide()
{
    std::cout << "\n\tWelcome to Tetris!\n";
    std::cout << "\n\tUse UP DOWN LEFT RIGHT keys to operate the tetromino!\n";
    std::cout << "\n\tPress any key to start a new game:";

    getch();
}

void Menu::print_result()
{
    std::cout << "\n\tOops! Game is over!\n";
    std::cout << "\n\tYour final score is: " << score << "\n";
    std::cout << "\n\tPress ENTER to start a new game:";

    while (getch() != '\r') {}
}

Menu::~Menu() {}