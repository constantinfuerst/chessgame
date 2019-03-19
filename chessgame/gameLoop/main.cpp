#include "pch.h"

#include "settings.h"
#include "gameInclude.h"

///////////////////////////////
// please check "settings.h" //
// for compiler settings	 //
///////////////////////////////

int MAIN {
	//TODO: more redable include structure (settings.h and cg_defs.h)
	//TODO: fix crash when exiting the application
	ENGINE gameEngine;
	return gameEngine.gameLoop();
}