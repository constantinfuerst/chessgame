#include "pch.h"
//change settings for compilation result in "settings.h"
#include "settings.h"
#include "gameInclude.h"

int MAIN {
	//TODO: more redable include structure (settings.h and cg_defs.h)
	//TODO: fix crash when exiting the application
	ENGINE gameEngine;
	return gameEngine.gameLoop();
}