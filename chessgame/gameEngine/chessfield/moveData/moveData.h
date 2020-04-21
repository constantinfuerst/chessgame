#pragma once
#include "pch.h"

#include "gameEngine/chessmen/chessmen.h"
#include "gameEngine/cg_defs.h"

class chessfield;

struct move {
	cg::color current_player = cg::white;
	typedef std::vector<std::unique_ptr<chessmen>> chessboard;

	struct chessmenMoved {
		bool hasmovedold;
		cg::chessfigure figure;
		cg::color player;
		cg::position oldPosition;
		cg::moveType move;
		cg::position newPosition;
	};

	std::vector<chessmenMoved> changes;
	void pushmove(const chessmenMoved& movedata);
	void makemove(cg::position oldPosition, cg::position newPosition, cg::color player, cg::chessfigure figure,
	              bool hasmovedold, cg::moveType move);
	void makemove(chessmen* chessmen, cg::position oldPosition, cg::position newPosition, bool hasmovedold,
	              cg::moveType move);
	static void removeChessmen(chessboard& onfield, cg::position pos);
	static void removeChessmen(chessboard& onside, cg::position pos, cg::chessfigure& figure);
	static void moveBack(chessboard& onfield, cg::position oldpos, cg::position currentpos, bool hasmovedold);
	move(const move& mve);
	move() = default;
};
