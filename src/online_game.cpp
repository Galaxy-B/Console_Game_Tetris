#include "online_game.hpp"

// function for the thread
void communicate(SOCKET client_socket, int& score, int& rival_score, std::atomic<bool>& gameover, std::atomic<bool>& match_over);

// check if any mistake happens, if so print it to console and shut down the program
void error_check(int ret, std::string alert);

Online_Game::Online_Game(int mode) : Game(mode)
{
    // initialization
    rival_score = 0;

    match_over.store(false);
    th_gameover.store(false);

    // buffer for socket IO
    char buffer[MAXLEN];

    // ret holds the return value when we call APIs
    int ret;
    
    // initialize network environment
    ret = WSAStartup(MAKEWORD(2, 2), &wsadata);

    // create the socket of the client
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    error_check((int)client_socket, "fail to create socket!");

    // connect the socket to the address of the server
    sockaddr_in address;
    address.sin_family = AF_INET;                                   // IPv4
    address.sin_port = htons(9999);                      // port 9999
    address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");      // IP address

    std::cout << "ready to connect to the server" << std::endl;

    ret = connect(client_socket, (sockaddr*)&address, sizeof(address));
    error_check(ret, "fail to connect to the server!");

    // print log
    std::cout << "wait for the rival to join..." << std::endl;

    // wait until the server notice us to start the game
    while (strcmp(buffer, "start") != 0) 
    {
        ret = recv(client_socket, buffer, MAXLEN, 0);
        error_check(ret, "fail to receive message from server!");
    }

    system("cls");

    // create a thread to keep communicating with the server
    std::thread update_score(communicate, client_socket, std::ref(score), std::ref(rival_score), std::ref(th_gameover), std::ref(match_over));

    update_score.detach();
}

std::pair<int, int> Online_Game::action()
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
        painter->paint(field, curr_tetromino, next_tetromino, score, rival_score);
    }

    // notice the server that our game is over
    th_gameover.store(true);

    // waiting for our rival to finish the game
    while (!match_over.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        painter->paint(field, curr_tetromino, next_tetromino, score, rival_score);
    }

    // return the score of this game
    return {score, rival_score};
}

Online_Game::~Online_Game() {}

void communicate(SOCKET client_socket, int& score, int& rival_score, std::atomic<bool>& gameover, std::atomic<bool>& match_over)
{
    int ret;
    // buffer for socket IO
    char buffer[MAXLEN];
    // until the server notice us that the game is over
    while (strcmp(buffer, "close") != 0)
    {   
        // send special message to the server when our game is over
        if (gameover.load())
        {
            std::sprintf(buffer, "over");
            // avoid sending "over" to server repeatedly
            gameover.store(false);
        }
        // otherwise send our score to the server
        else
        {
            std::sprintf(buffer, "%d", score);
        }
        
        ret = send(client_socket, buffer, MAXLEN, 0);
        error_check(ret, "fail to send score message to the server!");

        // receive our rival's score from the server
        ret = recv(client_socket, buffer, MAXLEN, 0);
        error_check(ret, "fail to receive score message from server!");
        std::sscanf(buffer, "%d", &rival_score);            
    }

    // release the network environment
    closesocket(client_socket);
    WSACleanup();

    // set semaphore match_over as TRUE
    match_over.store(true);
}

void error_check(int ret, std::string alert)
{
    // Oops! we meet mistakes
    if (ret == -1)
    {
        // print details to console and release the network environment
        std::cout << alert << std::endl;
        WSACleanup();
        
        system("pause");
        exit(-1);
    }
}