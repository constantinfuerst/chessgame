#include "pch.h"

#include "settings.h"
#include "gameInclude.h"

///////////////////////////////
// please check "settings.h" //
// for compiler settings	 //
///////////////////////////////

int MAIN {
	//TODO: fix crash when exiting the application (hard to replicate, no idea what causes it)
	ENGINE gameEngine;
	return gameEngine.gameLoop();
}