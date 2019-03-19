#include "pch.h"

#include "settings.h"
#include "gameInclude.h"

///////////////////////////////
// please check "settings.h" //
// for compiler settings	 //
///////////////////////////////

int MAIN {
	//TODO: fix crash when exiting the application (hard to replicate, no idea what causes it)
	//TODO: fix check system of the king (enables to enter a check without detection)
	ENGINE gameEngine;
	return gameEngine.gameLoop();
}