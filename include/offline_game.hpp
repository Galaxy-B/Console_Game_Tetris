#ifndef _OFFLINE_GAME_HPP_
#define _OFFLINE_GAME_HPP_

#include <cmath>
#include <vector>
#include <chrono>
#include <utility>
#include <mingw.thread.h>
#include "field.hpp"
#include "painter.hpp"
#include "tetromino.hpp"

/**************************************************************  

 * This header file states GAME class

 * GAME class is designed to implement basic game logic 

***************************************************************/

class Game
{
protected:
    // bias that indicates directions ( ← | ↓ | → | rotate)
    const int bias[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, 0}};

    // directions
    enum directions{LEFT, DOWN, RIGHT, ROTATE};
protected:
    // mark whether the game is over
    bool is_gameover;

    // play field of the game
    Field* field;

    // painter (take the blame for rendering)
    Painter* painter;

    // creator of tetrominos
    Tetromino_Creator* tetromino_creator;

    // the tetromino which is falling currently
    Tetromino* curr_tetromino;

    // the tetromino which is coming next
    Tetromino* next_tetromino;

    // a vector that contains lately eliminated line numbers
    std::vector<int> eliminated_lines;

    // total score
    int score;

    // threshold controls the falling speed of tetromino
    int threshold;

    // flags that indicates whether this key is being pressed constantly
    bool l_pressed = false;
    bool u_pressed = false;
    bool r_pressed = false;
    bool d_pressed = false;

public:
    Game(int mode);

    virtual std::pair<int, int> action();

    void gameover_check();

    bool collision_check(int direction);

    void tetromino_move(bool* key_down);

    void lockdown();

    void elimination_check();

    void tetromino_fall(int& tick_count);

    ~Game();
};

#endif