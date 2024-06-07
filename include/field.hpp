#ifndef _FIELD_HPP_
#define _FIELD_HPP_

#include <vector>

/**************************************************************  

 * This header file implements the play field

***************************************************************/

// different status of field units
#define SPACE 0         // ' '
#define BLOCK 1         // 'X'
#define BORDER 2        // '#'
#define ELIMINATED 3    // '='
#define GUIDELINE 4     // '|'

class Field
{
public:
    // paramaters about the size of the play field
    static const int FIELD_WIDTH = 12;
    static const int FIELD_HEIGHT = 22;

private:
    // use a two-dimension array to store the information of field
    std::vector<std::vector<int>*>* play_field;

public:
    Field();

    int get_play_field_unit(int x, int y);

    void set_play_field_unit(int x, int y, int value);

    ~Field();
};

#endif