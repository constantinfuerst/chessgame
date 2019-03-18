#include "pch.h"
//change settings for compilation result in "settings.h"
#include "settings.h"
#include "gameInclude.h"

#pragma warning(suppress: 28251)
int MAIN {
	ENGINE gameEngine;
	return gameEngine.gameLoop();
}