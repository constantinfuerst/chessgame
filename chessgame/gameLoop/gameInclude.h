#pragma once

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
	#else
		#include "consoleRenderer.h"
		#include "consoleRenderer.cpp"
		#pragma comment(linker, "/subsystem:console")
		#define MAIN main()
		#define ENGINE consoleRenderer
	#endif
#endif