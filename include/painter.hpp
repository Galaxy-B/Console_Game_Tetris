#ifndef _PAINTER_HPP_
#define _PAINTER_HPP_

#include <cwchar>
#include <vector>
#include <utility>
#include <string>
#include <windows.h>
#include "field.hpp"
#include "tetromino.hpp"

/**************************************************************  

 * This header file implements the painter

 * painter takes blame for rendering

***************************************************************/

class Painter
{
private:
    // parameters about the size of the screen
    const int SCREEN_WIDTH = 120;
    const int SCREEN_HEIGHT = 30;
    
    // hardcore all content type it may display on the screen
    const wchar_t* contents = L" X#=|";

    // notice the player of the next tetromino
    const wchar_t* t_notice = L"next:";

    // display the score that we have got
    const wchar_t* s_notice = L"score:";

    // everything we are going to paint
    wchar_t* screen;

    // coordinates of the guidelines painted last time
    std::vector<std::pair<int, int>> guidelines;

    // handle of the screen buffer
    HANDLE console_buffer;

    // record the actual amount of the bytes written to buffer
    DWORD bytes_written;

public:
    Painter();

    void paint(Field* field, Tetromino* curr_tetromino, Tetromino* next_tetromino, int score);

    ~Painter();
};

#endif