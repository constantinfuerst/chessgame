#include "pch.h"
#include "chessmen.h"

//base class function declarations

bool chessmen::validpos(chessmen::position position) {
	if (position.x >= fieldsize_x_start && position.x <= fieldsize_x_end && position.y >= fieldsize_y_start && position.y <= fieldsize_y_end) {
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
		if (chessmen->at(i)->board_position.x == pos.x && chessmen->at(i)->board_position.y == pos.y) {
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
	board_position.x = position_input.x;
	board_position.y = position_input.y;
	player_color = color_input;
	hasMoved = moved;
}