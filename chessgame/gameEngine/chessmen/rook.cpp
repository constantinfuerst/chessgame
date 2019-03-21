#include "pch.h"
#include "chessmen.h"

//function definitions for the rook

chessmen* rook::clone() const {
	return new rook(*this);
}

cg::chessfigure rook::figure() {
	return cg::rook;
}

//returns the legal moves of a rook on a specific board
std::vector<cg::position> rook::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	{
		{
			//walk to the right
			unsigned int x = board_position.x + 1;
			while (x <= cg::fieldsize_x_end) {
				const cg::position pos = { x, board_position.y };
				if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x++;
			}
		}
		{
			//walk to the left
			unsigned int x = board_position.x - 1;
			while (x >= cg::fieldsize_x_start) {
				const cg::position pos = { x, board_position.y };
				if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x--;
			}
		}
		{
			//walk to the bottom
			unsigned int y = board_position.y + 1;
			while (y <= cg::fieldsize_y_end) {
				const cg::position pos = { board_position.x, y };
				if (positiocheck(chessmen, pos, player_color) == cg::friendly_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				y++;
			}
		}
		{
			//walk to the top
			unsigned int y = board_position.y - 1;
			while (y >= cg::fieldsize_y_start) {
				const cg::position pos = { board_position.x, y };
				if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == cg::enemy_pos && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				y--;
			}
		}
	}
	return returnpos;
}
