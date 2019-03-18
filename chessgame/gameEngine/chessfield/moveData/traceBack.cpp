#include "pch.h"
#include "moveData.h"

void move::removeChessmen(chessboard& onfield, const chessmen::position pos) {
	for (size_t i = 0; i < onfield.size(); i++) {
		if (onfield.at(i)->getPos().x == pos.x && onfield.at(i)->getPos().y == pos.y) {
			onfield.erase(onfield.begin() + i);
			break;
		}
	}
}

void move::removeChessmen(chessboard& onside, const chessmen::position pos, chessmen::chessfigure& figure) {
	for (size_t i = 0; i < onside.size(); i++) {
		if (onside.at(i)->getPos().x == pos.x && onside.at(i)->getPos().y == pos.y && onside.at(i)->figure() == figure) {
			onside.erase(onside.begin() + i);
			break;
		}
	}
}

void move::moveBack(chessboard& onfield, const chessmen::position oldpos, const chessmen::position currentpos, const bool hasmovedold) {
	for (auto& i : onfield) {
		if (i->getPos().x == currentpos.x && i->getPos().y == currentpos.y) {
			i->setPos(oldpos);
			i->setHasMoved(hasmovedold);
		}
	}
}