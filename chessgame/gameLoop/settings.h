/*
PROGRAMM METADATA:
	name:           chessgame
	description:    chessgame project I made to learn OOP, using sfml combined with tgui or a console as output
	copyright:      (c) 2019 Constantin Fürst constantin@fuersten.info
LICENSE:
   This work is created by Constantin Fürst.
   If you are unsure if your use of this project is permitted or you'd like to use it in a way prohibited by the following license, please message "constantin@fuersten.info".
   This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
   To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-nd/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
DISCLAIMER:
THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS 'AS IS' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
 */

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

#define WIN_CONSOLE_GUI

//sets the dimensions of the chessboard used, why would you want to change this? No idea ...
#define FIELDSIZE 7

//define the directories of the game files
#define GAME_DIR std::string(".")
#define ASSETS_DIR GAME_DIR + std::string("\\assets\\")
#define SAVE_DIR GAME_DIR + std::string("\\saves\\")