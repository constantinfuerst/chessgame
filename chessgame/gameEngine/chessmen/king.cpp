#include "pch.h"
#include "chessmen.h"

//function definitions for the king

chessmen* king::clone() const {
	return new king(*this);
}

chessmen::chessfigure king::figure() {
	return chessmen::king;
}

std::vector<chessmen::position> king::possibleMoves(chessboard* chessmen) {
	std::vector<position> returnpos;
	{
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (board_position.x + i <= fieldsize_x_end && board_position.y + j <= fieldsize_y_end) {
					const position pos = { board_position.x + i ,board_position.y + j };
					if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
						returnpos.push_back(pos);
					}
				}
			}
		}
	}
	return returnpos;
}
