#include "pch.h"
#include "moveData.h"

void move::makemove(chessmen::position oldPosition, chessmen::position newPosition, chessmen::color player, chessmen::chessfigure figure, bool hasmovedold, moveType move) {
	chessmenMoved change;
	change.hasmovedold = hasmovedold;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = player;
	change.figure = figure;
	changes.push_back(change);
}

void move::makemove(chessmen* chessmen, chessmen::position oldPosition, chessmen::position newPosition, bool hasmovedold, moveType move) {
	chessmenMoved change;
	change.hasmovedold = hasmovedold;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = chessmen->getPlayer();
	change.figure = chessmen->figure();
	changes.push_back(change);
}

void move::pushmove(const chessmenMoved& movedata) {
	changes.push_back(movedata);
}

move::move(const move& mve) {
	for (size_t i = 0; i < mve.changes.size(); i++) {
		chessmenMoved changepush;
		changepush.figure = mve.changes[i].figure;
		changepush.hasmovedold = mve.changes[i].hasmovedold;
		changepush.player = mve.changes[i].player;
		changepush.oldPosition = mve.changes[i].oldPosition;
		changepush.move = mve.changes[i].move;
		changepush.newPosition = mve.changes[i].newPosition;
		changes.push_back(changepush);
	}
	current_player = mve.current_player;
}