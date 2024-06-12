#ifndef _ONLINE_GAME_HPP_
#define _ONLINE_GAME_HPP_

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define MAXLEN 0xff

#include <iostream>
#include <mingw.thread.h>
#include "offline_game.hpp"

/**************************************************************  

 * This header file states ONLINE_GAME class

 * ONLINE_GAME class extends GAME class with online battle function 

***************************************************************/

class Online_Game : public Game
{
private:
    WSADATA wsadata;

    SOCKET client_socket;

    sockaddr_in addr;

    char* buffer;

    int rival_score;

public:
    Online_Game(int mode);

    // override function action() so as to implement netword communication
    int action();

    ~Online_Game();
};

#endif