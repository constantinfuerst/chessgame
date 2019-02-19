#include "pch.h"
#include "consoleRenderer.h"

int main () {
	chessfield game;
	render_field(game.chessmen_onfield);
	std::cin.get();
}