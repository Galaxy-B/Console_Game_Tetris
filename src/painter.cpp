#include "painter.hpp"

Painter::Painter(int mode)
{
    // allocate memory for screen array
    screen = new wchar_t[SCREEN_HEIGHT * SCREEN_WIDTH];

    // initialize the screen
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
    {
        screen[i] = L' ';
    }

    // print the prompt of our play field
    swprintf(screen + (3 * SCREEN_WIDTH + 11), 16, L"Your Field");

    // print the prompt of the next tetromino
    swprintf(screen + (6 * SCREEN_WIDTH + 24), 16, L"next:");

    // print the prompt of the score that we've got
    swprintf(screen + (15 * SCREEN_WIDTH + 24), 16, L"score:");

    // print the prompt of our rival's score
    const wchar_t* field_prompt[2] = {L"", L"Your Rival"};
    swprintf(screen + (3 * SCREEN_WIDTH + 41), 16, field_prompt[mode]);

    // print the prompt of our rival's field
    const wchar_t* score_prompt[2] = {L"", L"score:"}; 
    swprintf(screen + (15 * SCREEN_WIDTH + 54), 16, score_prompt[mode]);

    // designate the capacity of guidelines
    guidelines.reserve(30);

    // fetch the handle of console buffer
    console_buffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

void Painter::init()
{
    // set handle as the active screen buffer
    SetConsoleActiveScreenBuffer(console_buffer);
}

// paint() for offline game
void Painter::paint(Field* field, Tetromino* curr_tetromino, Tetromino* next_tetromino, int score)
{
    // update screen array
    for (int i = 4; i < Field::FIELD_HEIGHT; i++)
    {
        for (int j = 0; j < Field::FIELD_WIDTH; j++)
        {
            screen[(i + 2) * SCREEN_WIDTH + (j + 10)] = contents[field->get_play_field_unit(j, i)];
        }
    }

    // render the current tetromino separately
    std::pair<int, int> last_unit = {0, 0};
    if (curr_tetromino != nullptr)
    {
        int tx = curr_tetromino->get_location_x();
        int ty = curr_tetromino->get_location_y();

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // don't render the part that hasn't left generating area
                if (ty + i >= 4 && curr_tetromino->get_rotated_unit(i, j) == 'X')
                {
                    screen[(ty + i + 2) * SCREEN_WIDTH + (tx + j + 10)] = contents[BLOCK];
                }
                // record the last 'X' unit of this tetromino
                last_unit = (curr_tetromino->get_rotated_unit(i, j) == 'X' ? std::make_pair(ty + i, tx + j) : last_unit);             
            }
        }

        // reset the guidelines painted last time
        for (auto guideline : guidelines)
        {
            screen[(guideline.first + 2) * SCREEN_WIDTH + (guideline.second + 10)] = contents[SPACE];
        }
        guidelines.clear();
        
        // render the new guidelines
        int gi = last_unit.first + 2;
        int gj = last_unit.second;
        for (int i = 0; gi + i < Field::FIELD_HEIGHT - 1; i++)
        {
            if (field->get_play_field_unit(gj, gi + i) == BLOCK || field->get_play_field_unit(gj, gi - 1) == BLOCK)
                break;
            
            // record this unit and change it into GUIDELINE
            guidelines.push_back(std::make_pair(gi + i, gj));
            screen[(gi + i + 2) * SCREEN_WIDTH + (gj + 10)] = contents[GUIDELINE];
        }
    }

    // render the next tetromino separately
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            screen[(8 + i) * SCREEN_WIDTH + (24 + j)] = (next_tetromino->get_rotated_unit(i, j) == 'X' ? contents[BLOCK] : contents[SPACE]);
        }
    }

    // render the score
    swprintf(screen + (17 * SCREEN_WIDTH + 24), 16, L"%d", score);

    // write everyting in screen array to console buffer
    WriteConsoleOutputCharacterW(console_buffer, screen, SCREEN_HEIGHT * SCREEN_WIDTH, {0, 0}, &bytes_written);
}

// paint() for online game
void Painter::paint(Field* field, Tetromino* curr_tetromino, Tetromino* next_tetromino, int score, int rival_score)
{
    // update screen array
    for (int i = 4; i < Field::FIELD_HEIGHT; i++)
    {
        for (int j = 0; j < Field::FIELD_WIDTH; j++)
        {
            screen[(i + 2) * SCREEN_WIDTH + (j + 10)] = contents[field->get_play_field_unit(j, i)];
        }
    }

    // render the current tetromino separately
    std::pair<int, int> last_unit = {0, 0};
    if (curr_tetromino != nullptr)
    {
        int tx = curr_tetromino->get_location_x();
        int ty = curr_tetromino->get_location_y();

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // don't render the part that hasn't left generating area
                if (ty + i >= 4 && curr_tetromino->get_rotated_unit(i, j) == 'X')
                {
                    screen[(ty + i + 2) * SCREEN_WIDTH + (tx + j + 10)] = contents[BLOCK];
                }
                // record the last 'X' unit of this tetromino
                last_unit = (curr_tetromino->get_rotated_unit(i, j) == 'X' ? std::make_pair(ty + i, tx + j) : last_unit);             
            }
        }

        // reset the guidelines painted last time
        for (auto guideline : guidelines)
        {
            screen[(guideline.first + 2) * SCREEN_WIDTH + (guideline.second + 10)] = contents[SPACE];
        }
        guidelines.clear();
        
        // render the new guidelines
        int gi = last_unit.first + 2;
        int gj = last_unit.second;
        for (int i = 0; gi + i < Field::FIELD_HEIGHT - 1; i++)
        {
            if (field->get_play_field_unit(gj, gi + i) == BLOCK || field->get_play_field_unit(gj, gi - 1) == BLOCK)
                break;
            
            // record this unit and change it into GUIDELINE
            guidelines.push_back(std::make_pair(gi + i, gj));
            screen[(gi + i + 2) * SCREEN_WIDTH + (gj + 10)] = contents[GUIDELINE];
        }
    }

    // render the next tetromino separately
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            screen[(8 + i) * SCREEN_WIDTH + (24 + j)] = (next_tetromino->get_rotated_unit(i, j) == 'X' ? contents[BLOCK] : contents[SPACE]);
        }
    }

    // render our score and our rival's
    swprintf(screen + (17 * SCREEN_WIDTH + 24), 16, L"%d", score);
    swprintf(screen + (17 * SCREEN_WIDTH + 54), 16, L"%d", rival_score);

    // write everyting in screen array to console buffer
    WriteConsoleOutputCharacterW(console_buffer, screen, SCREEN_HEIGHT * SCREEN_WIDTH, {0, 0}, &bytes_written);
}

Painter::~Painter()
{
    // close the screen buffer
    CloseHandle(console_buffer);

    // release contents of the screen
    delete screen;
}