#include "tetromino.hpp"

Tetromino_Creator::Tetromino_Creator()
{
    shapes = new std::vector<std::string*>(7);

    for (int i = 0; i < 7; i++)
    {
        shapes->at(i) = new std::string();
    }

    shapes->at(0)->append(".X..");      // shape1
    shapes->at(0)->append(".X..");
    shapes->at(0)->append(".X..");
    shapes->at(0)->append(".X..");
    
    shapes->at(1)->append("....");      // shape2
    shapes->at(1)->append(".X..");
    shapes->at(1)->append(".X..");
    shapes->at(1)->append(".XX.");
    
    shapes->at(2)->append("....");      // shape3
    shapes->at(2)->append(".X..");
    shapes->at(2)->append(".XX.");
    shapes->at(2)->append(".X..");
    
    shapes->at(3)->append("....");      // shape4
    shapes->at(3)->append(".XX.");
    shapes->at(3)->append(".X..");
    shapes->at(3)->append(".X..");
    
    shapes->at(4)->append("....");      // shape5
    shapes->at(4)->append("....");
    shapes->at(4)->append(".XX.");
    shapes->at(4)->append(".XX.");
    
    shapes->at(5)->append("....");      // shape6
    shapes->at(5)->append("..X.");
    shapes->at(5)->append(".XX.");
    shapes->at(5)->append(".X..");
    
    shapes->at(6)->append("....");      // shape7
    shapes->at(6)->append(".X..");
    shapes->at(6)->append(".XX.");
    shapes->at(6)->append("..X.");

    // allocate memory for helpers of generating random integer
    engine = new std::mt19937(time(nullptr));
    
    i_generator = new std::uniform_int_distribution<int>(0, 6);
    x_generator = new std::uniform_int_distribution<int>(0, 8);
}

// create a new tetromino randomly
Tetromino* Tetromino_Creator::create()
{
    int rand_i = (*i_generator)(*engine);
    int rand_x = (*x_generator)(*engine);

    // designate a random shape for this tetromino
    std::string* shape = new std::string(*(shapes->at(rand_i)));
    
    return new Tetromino(shape, rand_x, 1);
}

Tetromino_Creator::~Tetromino_Creator()
{
    // release all of the shapes
    for (int i = 0; i < 7; i++)
    {
        delete shapes->at(i);
    }
    delete shapes;

    // release helpers of generating random integer
    delete engine;
    delete i_generator;
    delete x_generator;
}