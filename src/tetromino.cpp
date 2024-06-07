#include "tetromino.hpp"

Tetromino::Tetromino(std::string* shape, int x, int y)
{
    this->shape = shape;
    this->location_x = x;
    this->location_y = y;
    this->rotation_angle = Tetromino::DEGREE_0;
}

// get the current value at tetromino[i][j] taking rotation into account
char Tetromino::get_rotated_unit(int i, int j)
{
    int idx = i * 4 + j;
    // cast the index into a corresponding one of the non-rotated itself
    switch(this->rotation_angle)
    {
        case DEGREE_0:
            return shape->at(idx);                              
        case DEGREE_90:
            return shape->at(12 - (idx % 4) * 4 + (idx / 4));   
        case DEGREE_180:
            return shape->at(15 - idx);                         
        case DEGREE_270:
            return shape->at(3 - (idx / 4) + (idx % 4) * 4);        
    }
    return 'X';
}

int Tetromino::get_location_x()
{
    return this->location_x;
}

int Tetromino::get_location_y()
{
    return this->location_y;
}

void Tetromino::forward()
{
    this->location_y++;
}

void Tetromino::shift_left()
{
    this->location_x--;
}

void Tetromino::shift_right()
{
    this->location_x++;
}

void Tetromino::rotate_left()
{
    this->rotation_angle = (this->rotation_angle - 1) % 4;
}

void Tetromino::rotate_right()
{
    this->rotation_angle = (this->rotation_angle + 1) % 4;
}

Tetromino::~Tetromino()
{
    delete shape;
}