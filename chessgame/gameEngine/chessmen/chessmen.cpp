#include "pch.h"
#include "chessmen.h"

bool chessmen::validpos(chessmen::position position) {
	if (position.x >= fieldsize_x_start && position.x <= fieldsize_x_end && position.y >= fieldsize_y_start && position.y <= fieldsize_y_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

chessmen::position_status chessmen::positiocheck(chessboard* chessmen, position pos, color player) {
	for (auto& i : *chessmen) {
		if (i->board_position.x == pos.x && i->board_position.y == pos.y) {
			if (i->player_color == player) {
				return friendly;
			}
			else {
				return enemy;
			}
		}
	}
	return empty;
}

chessmen::chessmen(color color_input, position position_input, bool moved) {
	board_position.x = position_input.x;
	board_position.y = position_input.y;
	player_color = color_input;
	hasMoved = moved;
}

chessmen::chessmen(color color_input, unsigned int posx, unsigned int posy, bool moved){
	board_position.x = posx;
	board_position.y = posy;
	player_color = color_input;
	hasMoved = moved;
}

chessmen::position chessmen::getPos() const {
	return this->board_position;
}

chessmen::color chessmen::getPlayer() const {
	return this->player_color;
}

bool chessmen::getHasMoved() const {
	return this->hasMoved;
}

void chessmen::setHasMoved(bool hasMoved) {
	this->hasMoved = hasMoved;
}

void chessmen::setPos(const position pos) {
	if (validpos(pos)) {
		this->board_position = pos;
	}
}

chessmen::position& chessmen::setPos() {
	return this->board_position;
}