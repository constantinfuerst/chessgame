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
	if (board_position.x + 2 <= fieldsize_x_end) {
		if (board_position.y + 1 <= fieldsize_y_end) {
			const position pos = { board_position.x + 2, board_position.y + 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.y - 1 >= fieldsize_y_end) {
			const position pos = { board_position.x + 2, board_position.y - 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (board_position.x - 2 >= fieldsize_x_start) {
		if (board_position.y + 1 <= fieldsize_y_end) {
			const position pos = { board_position.x - 2, board_position.y + 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.y - 1 >= fieldsize_y_start) {
			const position pos = { board_position.x - 2, board_position.y - 1 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (board_position.y + 2 <= fieldsize_y_end) {
		if (board_position.x + 1 <= fieldsize_x_end) {
			const position pos = { board_position.x + 1, board_position.y + 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.x - 1 >= fieldsize_x_start) {
			const position pos = { board_position.x - 1, board_position.y + 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (board_position.y - 2 >= fieldsize_y_start) {
		if (board_position.x + 1 <= fieldsize_x_end) {
			const position pos = { board_position.x + 1, board_position.y - 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.x - 1 >= fieldsize_x_start) {
			const position pos = { board_position.x - 1, board_position.y - 2 };
			if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	return returnpos;
}
