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

void move::placeBack(chessboard& onfield, chessboard& onside, const chessmen::position pos) {
	for (size_t i = 0; i < onside.size(); i++) {
		const chessmen::position boardpos = { onside.at(i)->board_position.x, onside.at(i)->board_position.y };
		if (boardpos.x == pos.x && boardpos.y == pos.y) {
			const chessmen::chessfigure type = onside.at(i)->figure();
			if (type == chessmen::rook)
				onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(onside.at(i)->player_color, onside.at(i)->board_position, TRUE)));
			else if (type == chessmen::knight)
				onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(onside.at(i)->player_color, onside.at(i)->board_position, TRUE)));
			else if (type == chessmen::pawn)
				onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<pawn>(onside.at(i)->player_color, onside.at(i)->board_position, TRUE)));
			else if (type == chessmen::bishop)
				onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(onside.at(i)->player_color, onside.at(i)->board_position, TRUE)));
			else if (type == chessmen::king)
				onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<king>(onside.at(i)->player_color, onside.at(i)->board_position, TRUE)));
			else
				onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(onside.at(i)->player_color, onside.at(i)->board_position, TRUE)));
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