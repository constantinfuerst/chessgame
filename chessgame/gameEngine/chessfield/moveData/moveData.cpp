#include "pch.h"
#include "moveData.h"

//create a move and add it to the current changes counter
void move::makemove(const cg::position oldPosition, const cg::position newPosition, const cg::color player, const cg::chessfigure figure, const bool hasmovedold, const cg::moveType move) {
	chessmenMoved change;
	change.hasmovedold = hasmovedold;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = player;
	change.figure = figure;
	changes.push_back(change);
}

//create a move and add it to the current changes counter
void move::makemove(chessmen* chessmen, const cg::position oldPosition, const cg::position newPosition, const bool hasmovedold, const cg::moveType move) {
	chessmenMoved change;
	change.hasmovedold = hasmovedold;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = chessmen->getPlayer();
	change.figure = chessmen->figure();
	changes.push_back(change);
}

//push a move to the current changes counter
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
