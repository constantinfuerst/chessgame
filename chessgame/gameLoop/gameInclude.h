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

////////////////////////////////////
// include and linker manager	  //
// for compiling in visual studio //
// in a windows environment		  //
////////////////////////////////////


//macro MAIN defines the entry point for the application
//macro ENGINE defines which of the two interface options should be used
//with sfmlRenderer being the GUI and consoleRenderer being the ASCII implementation respectively
//when using the GUI SFML 2.5.1 and the matching TGUI for SFML 2.5.1 must be provided
//the appropriate header definition files are required in "sfmlRenderer.h" and "cg_defs.h"

#ifdef _WIN32
	#ifdef WIN_DESKTOP_GUI
		#include <windows.h>
		#include "sfmlRenderer.h"
		#pragma comment(linker, "/subsystem:windows")
		#define MAIN APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
		#define ENGINE sfmlRenderer
		#pragma comment(linker, "/HEAP:33554432") //reserve 32MB heap space for maximum performance
	#elif defined WIN_CONSOLE_GUI
		#include "sfmlRenderer.h"
		#pragma comment(linker, "/subsystem:console")
		#define MAIN main()
		#define ENGINE sfmlRenderer
	#elif defined WIN_CONSOLE_ASCII
		#include "consoleRenderer.h"
		#include "consoleRenderer.cpp"
		#pragma comment(linker, "/subsystem:console")
		#define MAIN main()
		#define ENGINE consoleRenderer
	#endif
#endif