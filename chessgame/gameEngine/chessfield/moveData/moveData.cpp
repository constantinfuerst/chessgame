#include "pch.h"
#include "moveData.h"

void move::makemove(chessmen::position oldPosition, chessmen::position newPosition, chessmen::color player, chessmen::chessfigure figure, moveType move) {
	chessmenMoved change;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = player;
	change.figure = figure;
	changes.push_back(change);
}

void move::makemove(chessmen* chessmen, chessmen::position oldPosition, chessmen::position newPosition, moveType move) {
	chessmenMoved change;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = chessmen->player_color;
	change.figure = chessmen->figure();
	changes.push_back(change);
}

void move::pushmove(const chessmenMoved& movedata) {
	changes.push_back(movedata);
}