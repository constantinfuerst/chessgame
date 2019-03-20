#include "pch.h"
#include "chessmen.h"

//function definitions for the bishop

chessmen* bishop::clone() const {
	return new bishop(*this);
}

cg::chessfigure bishop::figure() {
	return cg::bishop;
}

std::vector<cg::position> bishop::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	{
		{
			unsigned int x = board_position.x + 1; unsigned int y = board_position.y + 1;
			while (x <= cg::fieldsize_x_end && y <= cg::fieldsize_y_end) {
				const cg::position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == cg::empty_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x++; y++;
			}
		}
		{
			unsigned int x = board_position.x - 1; unsigned int y = board_position.y + 1;
			while (x >= cg::fieldsize_x_start && y <= cg::fieldsize_y_end) {
				const cg::position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == cg::empty_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x--; y++;
			}
		}
		{
			unsigned int x = board_position.x + 1; unsigned int y = board_position.y - 1;
			while (x <= cg::fieldsize_x_end && y >= cg::fieldsize_y_start) {
				const cg::position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == cg::empty_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x++; y--;
			}
		}
		{
			unsigned int x = board_position.x - 1; unsigned int y = board_position.y - 1;
			while (x >= cg::fieldsize_x_start && y >= cg::fieldsize_y_start) {
				const cg::position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == cg::empty_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x--; y--;
			}
		}

	}
	return returnpos;
}
