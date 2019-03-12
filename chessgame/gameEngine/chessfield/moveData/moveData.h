#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"

class move {
public:
	chessmen::color current_player;
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
	typedef chessboard* ref_chessboard;
	typedef std::vector<ref_chessboard> chessfield_info;
	enum moveType {
		toempty = 0, toside = 1, newcm = 2
	};
	struct chessmenMoved {
		chessmen::chessfigure figure;
		chessmen::color player;
		chessmen::position oldPosition;
		moveType move;
		chessmen::position newPosition;
	};
	std::vector<chessmenMoved> changes;
	void pushmove(const chessmenMoved& movedata);
	void makemove(chessmen::position oldPosition, chessmen::position newPosition, chessmen::color player, chessmen::chessfigure figure, moveType move);
	void makemove(chessmen* chessmen, chessmen::position oldPosition, chessmen::position newPosition, moveType move);
	static void traceBack(chessfield_info& field, move* movetrace);
	static void removeChessmen(chessfield_info& board, chessmen::position pos);
	static void placeBack(chessfield_info& board, chessmen::position pos);
	static void moveBack(chessfield_info& board, chessmen::position oldpos, chessmen::position currentpos);
	move(const move& mve) {
		changes = mve.changes;
		current_player = mve.current_player;
	}
	move() = default;
};