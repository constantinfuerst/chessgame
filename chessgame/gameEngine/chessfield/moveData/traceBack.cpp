#include "pch.h"
#include "moveData.h"

void move::removeChessmen(chessboard& onfield, const chessmen::position pos) {
	for (size_t i = 0; i < onfield.size(); i++) {
		if (onfield.at(i)->board_position.x == pos.x && onfield.at(i)->board_position.y == pos.y) {
			onfield.erase(onfield.begin() + i);
			break;
		}
	}
}

void move::removeChessmen(chessboard& onside, const chessmen::position pos, chessmen::chessfigure& figure) {
	for (size_t i = 0; i < onside.size(); i++) {
		if (onside.at(i)->board_position.x == pos.x && onside.at(i)->board_position.y == pos.y && onside.at(i)->figure() == figure) {
			onside.erase(onside.begin() + i);
			break;
		}
	}
}

void move::moveBack(chessboard& onfield, const chessmen::position oldpos, const chessmen::position currentpos, const bool hasmovedold) {
	for (auto& i : onfield) {
		if (i->board_position.x == currentpos.x && i->board_position.y == currentpos.y) {
			i->board_position = oldpos;
			i->hasMoved = hasmovedold;
		}
	}
}