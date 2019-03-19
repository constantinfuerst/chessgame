#pragma once

#include "pch.h"
#include "gameEngine/chessfield/chessfield.h"
#include <iostream>

class consoleRenderer final{
protected:
	static cg::game_status processOutput(cg::full_game_status status);
	static unsigned int translateY(const unsigned int& org_y);
	static unsigned int translateX(const unsigned int& org_x);
	static cg::position strtopos(std::string input);
	static std::string postostr(cg::position pos);
	static void render(chessfield& game);

public:
	int gameLoop() const;
	~consoleRenderer()  = default;
	consoleRenderer() = default;
};
