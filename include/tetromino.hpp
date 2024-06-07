#ifndef _TETROMINO_HPP_
#define _TETROMINO_HPP_

#include <time.h>
#include <vector>
#include <string>
#include <random>

/**************************************************************  

 * This header file states classes about tetrominos

***************************************************************/

class Tetromino
{
public:
    // macros of the rotation angle
    static const int DEGREE_0   = 0;
    static const int DEGREE_90  = 1;
    static const int DEGREE_180 = 2;
    static const int DEGREE_270 = 3;

private:
    // use single_dimension container so as to rotate easily 
    std::string* shape;
    
    // coordinate of the upper left of this tetromino
    int location_x;
    int location_y;
    
    // current rotation angle of this tetromino
    int rotation_angle;

public:
    Tetromino(std::string* shape, int x, int y);

    char get_rotated_unit(int i, int j);

    int get_location_x();
    
    int get_location_y();

    void forward();

    void shift_left();

    void shift_right();

    void rotate_left();

    void rotate_right();

    ~Tetromino();
};

class Tetromino_Creator
{
private:
    // hardcore all seven kinds of shape for tetrominos
    std::vector<std::string*>* shapes;

    // objects for creating random integer
    std::mt19937* engine;
    
    std::uniform_int_distribution<int>* i_generator;     // generate random index
    std::uniform_int_distribution<int>* x_generator;     // generate random location_x
    
public:
    Tetromino_Creator();

    Tetromino* create();

    ~Tetromino_Creator();   
};

#endif