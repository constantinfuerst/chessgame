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
