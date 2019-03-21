#include "pch.h"
#include "chessmen.h"

//function definitions for the knight

chessmen* knight::clone() const {
	return new knight(*this);
}

cg::chessfigure knight::figure() {
	return cg::knight;
}

//returns the legal moves of a knight on a specific board
std::vector<cg::position> knight::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	if (board_position.x + 2 <= cg::fieldsize_x_end) {
		if (board_position.y + 1 <= cg::fieldsize_y_end) {
			const cg::position pos = { board_position.x + 2, board_position.y + 1 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.y - 1 >= cg::fieldsize_y_end) {
			const cg::position pos = { board_position.x + 2, board_position.y - 1 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (board_position.x - 2 >= cg::fieldsize_x_start) {
		if (board_position.y + 1 <= cg::fieldsize_y_end) {
			const cg::position pos = { board_position.x - 2, board_position.y + 1 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.y - 1 >= cg::fieldsize_y_start) {
			const cg::position pos = { board_position.x - 2, board_position.y - 1 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (board_position.y + 2 <= cg::fieldsize_y_end) {
		if (board_position.x + 1 <= cg::fieldsize_x_end) {
			const cg::position pos = { board_position.x + 1, board_position.y + 2 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.x - 1 >= cg::fieldsize_x_start) {
			const cg::position pos = { board_position.x - 1, board_position.y + 2 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	if (board_position.y - 2 >= cg::fieldsize_y_start) {
		if (board_position.x + 1 <= cg::fieldsize_x_end) {
			const cg::position pos = { board_position.x + 1, board_position.y - 2 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
		if (board_position.x - 1 >= cg::fieldsize_x_start) {
			const cg::position pos = { board_position.x - 1, board_position.y - 2 };
			if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
				returnpos.push_back(pos);
			}
		}
	}
	return returnpos;
}
