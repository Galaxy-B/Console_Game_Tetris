#ifndef _ONLINE_GAME_HPP_
#define _ONLINE_GAME_HPP_

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define MAXLEN 0xff

#include <iostream>
#include <utility>
#include <mingw.thread.h>
#include <atomic>
#include <functional>

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

    int rival_score;

    // thread for communicating with the server
    std::thread communication;

    // signal value to inform the thread of our game status
    std::atomic<bool> th_gameover;

public:
    Online_Game(int mode);

    // override function action() so as to implement network communication
    std::pair<int, int> action();

    ~Online_Game();
};

#endif