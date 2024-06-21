# Tetris OL

This lightweight project implements a classic e-game called Tetris. With the support of **double buffering technology**, it runs perfectly well in console.  
  
Except for basic game logic, this project supports **network coummunication** of **C/S framework** (a.k.a. Client/Server) using **Windows Socket API**, which means you can match with your friends through **WLAN**.  
  
You can find the implement of the *server* in `server` directory, and the implement of the *client* in `online_game.cpp` file under `src` directory.  

## Before Compile
* Check Header Files  
  
Due to the version of my MinGW compiler, **Thread** in C++ 11 standard is not supported natively. Therefore, you will find `<mingw.thread.h>` included wherever I want to deal with threads. If your compiler supports the usage of `<Thread>`, remember to change every single `<mingw.thread.h>` into `<Thread>`.  
  
* Rebind the IP address  
  
As I mentioned before, implements of the *server* and the *cilent* are in `main.cpp` under `server/src` directory and `src` directory respectively. Those are where **IP address** of the *server* gets binded as follow.  
```C++
// bind the socket to the address of the server
sockaddr_in address;  
address.sin_family = AF_INET;  
address.sin_port = htons(9999);  
address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
```  
  
If you want to run a locat test for this project, just apply `127.0.0.1` to the IP address. Otherwise you have to apply the IP address of the machine which runs the *server*.  
  
Remember that **both the *server* and the *client*** have to apply the change.  

## Compile
For convenience, I assume that you are at `.../Console_Game_Tetris` directory initially.  
  
This project uses **Cmake** to manage the source code, so type following commands to compile the client and the server seperately.
* *Client*  
```
mkdir build
cd build
cmake ..
make
```  
* *Server*
```
cd server
mkdir build
cd build
cmake ..
make
```  
  
If compiled successfully, you ought to find the *client* `Tetris.exe` under  `.../Console_Game_Tetris/build/output`, the *server* `My_Server.exe` in `.../Console_Game_Tetris/server/build/output`.

## Reminder

* Do remember to launch server ahead of joining a online match in client.  
  
* ***This project can be successfully executed on Windows system ONLY*** 
