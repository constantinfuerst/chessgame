#include "pch.h"

#include "chessmen.h"

//function definitions for the king

chessmen* king::clone() const {
	return new king(*this);
}

cg::chessfigure king::figure() {
	return cg::king;
}

//returns the legal moves of a king on a specific board
std::vector<cg::position> king::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	{
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (board_position.x + i <= cg::fieldsize_x_end && board_position.y + j <= cg::fieldsize_y_end) {
					const cg::position pos = {board_position.x + i, board_position.y + j};
					if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
						returnpos.push_back(pos);
					}
				}
			}
		}
	}
	return returnpos;
}
