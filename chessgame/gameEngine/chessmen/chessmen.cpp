#include "pch.h"
#include "chessmen.h"

//checks for a position to be valid (in the parameters of the currently used field)
bool chessmen::validpos(cg::position position) {
	if (position.x >= cg::fieldsize_x_start && position.x <= cg::fieldsize_x_end && position.y >= cg::fieldsize_y_start && position.y <= cg::fieldsize_y_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//checks a position for its state (empty, enemy or friendly)
cg::position_status chessmen::positiocheck(chessboard* chessmen, cg::position pos, cg::color player) {
	//for every chessmen on the field
	for (auto& i : *chessmen) {
		if (i->board_position.x == pos.x && i->board_position.y == pos.y) {
			if (i->player_color == player) {
				//if there is a chessmen with the same position as "pos" that is of the same color as "player" it must be a used position by a friendly
				return cg::friendly_pos;
			}
			else {
				//if there is a chessmen with the same position as "pos" that is of a different color than "player" it must be a used position by an enemy
				return cg::enemy_pos;
			}
		}
	}
	//if no chessmen with the same position was found then it must be empty
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

//getter for the position of a chessmen
cg::position chessmen::getPos() const {
	return this->board_position;
}

//getter for the player of a chessmen
cg::color chessmen::getPlayer() const {
	return this->player_color;
}

//getter for the hasMoved state of a chessmen
bool chessmen::getHasMoved() const {
	return this->hasMoved;
}

//setter for the hasMoved state of a chessmen
void chessmen::setHasMoved(bool hasMoved) {
	this->hasMoved = hasMoved;
}

//setter for the position of a chessmen taking in a new position
void chessmen::setPos(const cg::position pos) {
	if (validpos(pos)) {
		this->board_position = pos;
	}
}

//getter for the position of a chessmen returning a refference to manually set it
cg::position& chessmen::setPos() {
	return this->board_position;
}