#include "pch.h"

#include "settings.h"
#include "gameInclude.h"

///////////////////////////////
// please check "settings.h" //
// for compiler settings	 //
///////////////////////////////

int MAIN {
	//TODO: fix crash when exiting the application (hard to replicate, no idea what causes it)
	//TODO: merge two functions of "moveCasteling" into one function
	//TODO: debug "sfmlRenderFunc" -> why dont the UI elements get the correct size
	ENGINE gameEngine;
	return gameEngine.gameLoop();
}