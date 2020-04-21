#include "pch.h"

#include "moveData.h"

//removes a chessmen on a specified position from the normal field
void move::removeChessmen(chessboard& onfield, const cg::position pos) {
	for (size_t i = 0; i < onfield.size(); i++) {
		if (onfield.at(i)->getPos().x == pos.x && onfield.at(i)->getPos().y == pos.y) {
			onfield.erase(onfield.begin() + i);
			break;
		}
	}
}

//removes a specified chess piece that was last positioned on a specified position from the side
void move::removeChessmen(chessboard& onside, const cg::position pos, cg::chessfigure& figure) {
	for (size_t i = 0; i < onside.size(); i++) {
		if (onside.at(i)->getPos().x == pos.x && onside.at(i)->getPos().y == pos.y && onside.at(i)->figure() == figure
		) {
			onside.erase(onside.begin() + i);
			break;
		}
	}
}

//moves a chessmen back
void move::moveBack(chessboard& onfield, const cg::position oldpos, const cg::position currentpos,
                    const bool hasmovedold) {
	for (auto& i : onfield) {
		if (i->getPos().x == currentpos.x && i->getPos().y == currentpos.y) {
			i->setPos(oldpos);
			i->setHasMoved(hasmovedold);
		}
	}
}
