#include "pch.h"
#include "chessmen.h"

//base class function declarations

bool chessmen::validpos(chessmen::position position) {
	if (position[0] >= fieldsize_x_start && position[0] <= fieldsize_x_end && position[1] >= fieldsize_y_start && position[1] <= fieldsize_y_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

std::vector<chessmen::position> chessmen::possibleMoves(chessboard* chessmen) {
	std::vector<position> emptyreturn;
	return emptyreturn;
}

chessmen::position_status chessmen::positiocheck(chessboard* chessmen, position pos, color player) {
	for (size_t i = 0; i < chessmen->size(); i++) {
		if (chessmen->at(i)->current_position[0] == pos[0] && chessmen->at(i)->current_position[1] == pos[1]) {
			if (chessmen->at(i)->player_color == player) {
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
	current_position[0] = position_input[0];
	current_position[1] = position_input[1];
	player_color = color_input;
	hasMoved = moved;
}