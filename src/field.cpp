#include "field.hpp"

Field::Field()
{
    // initialize the play field by setting every unit as space
    play_field = new std::vector<std::vector<int>*>(FIELD_HEIGHT);
    
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        play_field->at(i) = new std::vector<int>(FIELD_WIDTH, SPACE);
    }

    // change the vertical boundary into border
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        play_field->at(i)->at(0) = BORDER;
        play_field->at(i)->at(FIELD_WIDTH - 1) = BORDER;
    }
    
    // change the horizontal boundary into border
    for (int j = 0; j < FIELD_WIDTH; j++)
    {
        play_field->at(FIELD_HEIGHT - 1)->at(j) = BORDER;
    }
}

int Field::get_play_field_unit(int x, int y)
{
    // boundary check
    if (x < 0 || x >= FIELD_WIDTH || y < 0 || y >= FIELD_HEIGHT)
    {
        return BORDER;
    }
    else
    {
        return play_field->at(y)->at(x);
    }    
}

void Field::set_play_field_unit(int x, int y, int value)
{
    // boundary check
    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT)
    {
        play_field->at(y)->at(x) = value;
    }
}

Field::~Field()
{
    // delete every line of the play field
    for (int i = 0; i < FIELD_HEIGHT; i++)
    {
        delete play_field->at(i);
    }
    // delete the play field itself
    delete play_field;
}