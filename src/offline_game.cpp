#include "offline_game.hpp"

Game::Game(int mode)
{
    // initialization
    is_gameover = false;
    score = 0;

    // allocate memory
    field = new Field();
    painter = new Painter(mode);
    tetromino_creator = new Tetromino_Creator();

    // designate the capacity of eliminated_lines
    eliminated_lines.reserve(8);

    // set both tetromino pointer as nullptr
    curr_tetromino = nullptr;
    next_tetromino = nullptr;

    // set falling speed as 1 unit per 500 ms
    threshold = 10;
}

// action() start to execute the main loop of the game
int Game::action()
{   
    painter->init();
    
    // monitor the status of key ← ↑ → ↓
    bool key_down[4] = {false};

    // counting down for current tetromino to fall
    int tick_count = 0; 

    // total amount of the generated tetromino
    int tetromino_count = 0;

    int round = 0;

    // fetch a random tetromino for next_tetromino
    next_tetromino = tetromino_creator->create();
    
    // main logic of the game
    while (!is_gameover)
    {
        /*
         * game tick
         */
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        /*
         * if the falling tetromino has been locked
         */
        if (curr_tetromino == nullptr)
        {
            tetromino_count++;
            // convert the next_tetromino as the curr one
            curr_tetromino = next_tetromino;
            // fetch the next with a randomly new one
            next_tetromino = tetromino_creator->create();
            // if the latest generated tetromino can't fall the game is over
            gameover_check();
        }

        /*
         * increase the falling speed base on the total amount of tetrominos 
        */
        if (tetromino_count / 20 > round && threshold > 5)
        {
            threshold--;
            round++;
        }

        /*
         * get the asynchronous key board input
         */
        for (int i = 0; i < 4; i++)
        {                                                             // ←   ↑   →   ↓
            key_down[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x26\x27\x28"[i]))) != 0;
        }

        /*
         * react to user input
         */
        tetromino_move(key_down);

        /*
         * the current tetromino should fall periodically
         */
        tetromino_fall(tick_count);

        /* 
         * render the current window
         */
        painter->paint(field, curr_tetromino, next_tetromino, score);
    }

    // return the score of this game
    return score;
}

void Game::gameover_check()
{
    // if the latest generated tetromino can't fall the game is over
    is_gameover = collision_check(DOWN);
}

bool Game::collision_check(int direction)
{   
    // coordinate of the upper left of this tetromino (y -> i | x -> j)
    int tx = curr_tetromino->get_location_x();
    int ty = curr_tetromino->get_location_y();

    // coordinate of the biased position in play field
    int fx = tx + bias[direction][0];
    int fy = ty + bias[direction][1];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // this unit in tetromino 
            char tunit = curr_tetromino->get_rotated_unit(i, j); 
            // this unit in play field
            int funit = field->get_play_field_unit(fx + j, fy + i);

            // check if this biased unit in play field fits the current tetromino
            if (tunit == 'X' && funit != SPACE)
                return true;
        }
    }
    return false;
}

void Game::tetromino_move(bool* key_down)
{
    if (key_down[0])                                                        // ←
    {
        if (!collision_check(LEFT) && !l_pressed)
        {
            curr_tetromino->shift_left();
        }
        l_pressed = true;
    }
    else
    {
        l_pressed = false;
    }
    
    if (key_down[2])                                                        // →
    {
        if (!collision_check(RIGHT) && !r_pressed)
        {
            curr_tetromino->shift_right();
        }
        r_pressed = true;
    }
    else
    {
        r_pressed = false;
    }

    if (key_down[3])                                                        // ↓
    {
        // the tetromino will keep falling until it can't do more 
        while (!d_pressed && !collision_check(DOWN))
        {
            curr_tetromino->forward();
        }
        d_pressed = true;
    }
    else
    {
        d_pressed = false;
    }

    if (key_down[1])                                                        // ↑
    {
        if (!u_pressed)
        {
            // first get this tetromino rotate 90 degree
            curr_tetromino->rotate_right();
            // if this operation is not allowed then rotate it backward
            if (collision_check(ROTATE))
            {
                curr_tetromino->rotate_left();
            }
        }
        u_pressed = true;
    }
    else
    {
        u_pressed = false;
    }
}

void Game::lockdown()
{
    // coordinate of the upper left of this tetromino (y -> i | x -> j)
    int tx = curr_tetromino->get_location_x();
    int ty = curr_tetromino->get_location_y();

    // write this tetromino into our play field
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (curr_tetromino->get_rotated_unit(i, j) == 'X')
            {
                field->set_play_field_unit(tx + j, ty + i, BLOCK);
            }
        }
    }
}

void Game::elimination_check()
{
    // apparently we only have to check four lines where the current tetromino got locked
    int ty = curr_tetromino->get_location_y();

    for (int i = 0; i < 4; i++)
    {
        bool is_stuffed = true;
        // avoid checking the border
        for (int j = 1; j < Field::FIELD_WIDTH - 1; j++)
        {
            is_stuffed &= (field->get_play_field_unit(j, ty + i) == BLOCK);
        }

        if (is_stuffed)
        {
            // change the content of each unit in this line into '='(ELIMINATED)
            for (int j = 1; j < Field::FIELD_WIDTH - 1; j++)
            {
                field->set_play_field_unit(j, ty + i, ELIMINATED);
            }
            // record this line
            eliminated_lines.push_back(ty + i);
        }
    }

    if (!eliminated_lines.empty())
    {
        // calculate the score that we got from this elimination
        score += std::pow(eliminated_lines.size(), 2);
        
        // if there is any line got eliminated, render the screen to display it
        painter->paint(field, nullptr, next_tetromino, score);
        // display it for a while
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // update our play field so as to remove those eliminated lines
        for (int line : eliminated_lines)
        {
            for (int j = 1; j < Field::FIELD_WIDTH - 1; j++)
            {
                for (int i = line; i > 3; i--)
                {
                    field->set_play_field_unit(j, i, field->get_play_field_unit(j, i - 1));
                }
                field->set_play_field_unit(j, 3, SPACE);
            }
        }
        // render the updated play field
        painter->paint(field, nullptr, next_tetromino, score);

        eliminated_lines.clear();
    }
}

void Game::tetromino_fall(int& tick_count)
{
    // check if the tick counter has reached the threshold
    if (tick_count == threshold)
    {
        // check if the current tetromino is allowed to fall 1 unit
        if (!collision_check(DOWN))
        {
            curr_tetromino->forward();
        }
        // otherwise lock it down at its current position in our play field
        else
        {
            lockdown();
            // check if this lockdown managed to eliminate any line
            elimination_check();

            // delete this tetromino
            delete curr_tetromino;
            curr_tetromino = nullptr;
        }
    }

    // update the tick counter
    tick_count = (tick_count + 1) % (threshold + 1);
}

Game::~Game()
{
    // release those allocated memory
    delete field;
    delete painter;
    delete tetromino_creator;

    // the current trtromino must have been deleted
    delete next_tetromino;
}