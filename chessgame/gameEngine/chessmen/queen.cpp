#include "pch.h"
#include "chessmen.h"

//function definitions for the queen

chessmen* queen::clone() const {
	return new queen(*this);
}

chessmen::chessfigure queen::figure() {
	return chessmen::queen;
}

std::vector<chessmen::position> queen::possibleMoves(chessboard* chessmen) {
	std::vector<position> returnpos;
	{
		{
			unsigned int x = board_position.x + 1; unsigned int y = board_position.y + 1;
			while (x <= fieldsize_x_end && y <= fieldsize_y_end) {
				const position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
			while (x >= fieldsize_x_start && y <= fieldsize_y_end) {
				const position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
			while (x <= fieldsize_x_end && y >= fieldsize_y_start) {
				const position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
			while (x >= fieldsize_x_start && y >= fieldsize_y_start) {
				const position pos = { x, y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
					returnpos.push_back(pos);
					break;
				}
				else {
					break;
				}
				x--; y--;
			}
		}
		{
			unsigned int x = board_position.x + 1;
			while (x <= fieldsize_x_end) {
				const position pos = { x, board_position.y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
			unsigned int x = board_position.x - 1;
			while (x >= fieldsize_x_start) {
				const position pos = { x, board_position.y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
			unsigned int y = board_position.y + 1;
			while (y <= fieldsize_y_end) {
				const position pos = { board_position.x, y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
			unsigned int y = board_position.y - 1;
			while (y >= fieldsize_y_start) {
				const position pos = { board_position.x, y };
				if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
					returnpos.push_back(pos);
				}
				else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
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
