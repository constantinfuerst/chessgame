#include "pch.h"

#include "chessmen.h"

//function definitions for the queen

chessmen* queen::clone() const {
	return new queen(*this);
}

cg::chessfigure queen::figure() {
	return cg::queen;
}

//returns the legal moves of a queen on a specific board
std::vector<cg::position> queen::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	{
		{
			//walk to the bottom right corner
			unsigned int x = board_position.x + 1;
			unsigned int y = board_position.y + 1;
			while (x <= cg::fieldsize_x_end && y <= cg::fieldsize_y_end) {
				const cg::position pos = {x, y};
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
				x++;
				y++;
			}
		}
		{
			//walk to the bottom left corner
			unsigned int x = board_position.x - 1;
			unsigned int y = board_position.y + 1;
			while (x >= cg::fieldsize_x_start && y <= cg::fieldsize_y_end) {
				const cg::position pos = {x, y};
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
				x--;
				y++;
			}
		}
		{
			//walk to the top right corner
			unsigned int x = board_position.x + 1;
			unsigned int y = board_position.y - 1;
			while (x <= cg::fieldsize_x_end && y >= cg::fieldsize_y_start) {
				const cg::position pos = {x, y};
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
				x++;
				y--;
			}
		}
		{
			//walk to the top left corner
			unsigned int x = board_position.x - 1;
			unsigned int y = board_position.y - 1;
			while (x >= cg::fieldsize_x_start && y >= cg::fieldsize_y_start) {
				const cg::position pos = {x, y};
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
				x--;
				y--;
			}
		}
		{
			//walk to the right
			unsigned int x = board_position.x + 1;
			while (x <= cg::fieldsize_x_end) {
				const cg::position pos = {x, board_position.y};
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
				x++;
			}
		}
		{
			//walk to the left
			unsigned int x = board_position.x - 1;
			while (x >= cg::fieldsize_x_start) {
				const cg::position pos = {x, board_position.y};
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
				x--;
			}
		}
		{
			//walk down
			unsigned int y = board_position.y + 1;
			while (y <= cg::fieldsize_y_end) {
				const cg::position pos = {board_position.x, y};
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
				y++;
			}
		}
		{
			//walk up
			unsigned int y = board_position.y - 1;
			while (y >= cg::fieldsize_y_start) {
				const cg::position pos = {board_position.x, y};
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
				y--;
			}
		}
	}
	return returnpos;
}
