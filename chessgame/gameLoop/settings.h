#pragma once

/////////////////////////////////////
// SETTINGS FOR COMPILATION RESULT //
/////////////////////////////////////

//this project should only require minimal changes to work on UNIX-based systems and can be compiled for every system supported by the sfml libraries
//which is pretty much any os imagineable
//getting it to run will require a preprocessor block with the correct includes and settings
//to create your own settings profile add a preprocessor block in "gameInclude.h"
//check if "sfmlRenderer" should use "std::cout" (when in need of a console with "debug" info)
//or not and if so then add a preprocessor block in "sfmlRenderer.cpp"

// choose between three profiles if you are on Windows
// WIN_DESKTOP_GUI creates only the GUI game window
// WIN_CONSOLE_GUI creates a console debug window and GUI game window
// WIN_CONSOLE_ASCII creates only a console window with ASCII implementation

#define WIN_DESKTOP_GUI

#define FIELDSIZE 7

#define GAME_DIR std::string("C:\\Users\\Constantin\\source\\repos\\chessgame\\build")
#define ASSETS_DIR GAME_DIR + std::string("\\assets\\")
#define SAVE_DIR GAME_DIR + std::string("\\saves\\")