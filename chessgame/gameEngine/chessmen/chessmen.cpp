#include "pch.h"
#include "chessmen.h"

bool chessmen::validpos(cg::position position) {
	if (position.x >= fieldsize_x_start && position.x <= fieldsize_x_end && position.y >= fieldsize_y_start && position.y <= fieldsize_y_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

cg::position_status chessmen::positiocheck(chessboard* chessmen, cg::position pos, cg::color player) {
	for (auto& i : *chessmen) {
		if (i->board_position.x == pos.x && i->board_position.y == pos.y) {
			if (i->player_color == player) {
				return cg::friendly_pos;
			}
			else {
				return cg::enemy_pos;
			}
		}
	}
	return cg::empty_pos;
}

chessmen::chessmen(cg::color color_input, cg::position position_input, bool moved) {
	board_position.x = position_input.x;
	board_position.y = position_input.y;
	player_color = color_input;
	hasMoved = moved;
}

chessmen::chessmen(cg::color color_input, unsigned int posx, unsigned int posy, bool moved){
	board_position.x = posx;
	board_position.y = posy;
	player_color = color_input;
	hasMoved = moved;
}

cg::position chessmen::getPos() const {
	return this->board_position;
}

cg::color chessmen::getPlayer() const {
	return this->player_color;
}

bool chessmen::getHasMoved() const {
	return this->hasMoved;
}

void chessmen::setHasMoved(bool hasMoved) {
	this->hasMoved = hasMoved;
}

void chessmen::setPos(const cg::position pos) {
	if (validpos(pos)) {
		this->board_position = pos;
	}
}

cg::position& chessmen::setPos() {
	return this->board_position;
}