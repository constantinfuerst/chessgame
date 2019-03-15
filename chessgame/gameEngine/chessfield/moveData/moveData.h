#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"

class chessfield;

class move {
public:
	chessmen::color current_player;
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
	enum moveType {
		toempty = 0, toside = 1, newcm = 2
	};
	struct chessmenMoved {
		bool hasmovedold;
		chessmen::chessfigure figure;
		chessmen::color player;
		chessmen::position oldPosition;
		moveType move;
		chessmen::position newPosition;
	};
	std::vector<chessmenMoved> changes;
	void pushmove(const chessmenMoved& movedata);
	void makemove(chessmen::position oldPosition, chessmen::position newPosition, chessmen::color player, chessmen::chessfigure figure, bool hasmovedold, moveType move);
	void makemove(chessmen* chessmen, chessmen::position oldPosition, chessmen::position newPosition, bool hasmovedold, moveType move);
	static void removeChessmen(chessboard& onfield, chessmen::position pos);
	static void placeBack(chessboard& onfield, chessboard& onside, chessmen::position pos);
	static void moveBack(chessboard& onfield, chessmen::position oldpos, chessmen::position currentpos, bool hasmovedold);
	move(const move& mve);
	move() = default;
};
