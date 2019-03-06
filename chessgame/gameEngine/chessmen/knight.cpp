#include "pch.h"
#include "chessmen.h"

//function definitions for the knight

chessmen* knight::clone() const {
	return new knight(*this);
}

chessmen::chessfigure knight::figure() {
	return chessmen::knight;
}

std::vector<chessmen::position> knight::possibleMoves(chessboard* chessmen) {
	std::vector<position> returnpos;
	if (current_position[0] + 2 <= fieldsize_x_end) {
		if (current_position[1] + 1 <= fieldsize_y_end) {
			const position pos = { current_position[0] + 2, current_position[1] + 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (current_position[1] - 1 >= fieldsize_y_end) {
			const position pos = { current_position[0] + 2, current_position[1] - 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (current_position[0] - 2 >= fieldsize_x_start) {
		if (current_position[1] + 1 <= fieldsize_y_end) {
			const position pos = { current_position[0] - 2, current_position[1] + 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (current_position[1] - 1 >= fieldsize_y_start) {
			const position pos = { current_position[0] - 2, current_position[1] - 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (current_position[1] + 2 <= fieldsize_y_end) {
		if (current_position[0] + 1 <= fieldsize_x_end) {
			const position pos = { current_position[0] + 1, current_position[1] + 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (current_position[0] - 1 >= fieldsize_x_start) {
			const position pos = { current_position[0] - 1, current_position[1] + 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (current_position[1] - 2 >= fieldsize_y_start) {
		if (current_position[0] + 1 <= fieldsize_x_end) {
			const position pos = { current_position[0] + 1, current_position[1] - 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (current_position[0] - 1 >= fieldsize_x_start) {
			const position pos = { current_position[0] - 1, current_position[1] - 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	return returnpos;
}
