#include "pch.h"
#include "chessmen.h"

//function definitions for the bishop

chessmen* bishop::clone() const {
	return new bishop(*this);
}

chessmen::chessfigure bishop::figure() {
	return chessmen::bishop;
}

std::vector<chessmen::position> bishop::possibleMoves(chessboard* chessmen) {
	std::vector<position> returnpos;
	{
		{
			int x = current_position[0] + 1; int y = current_position[1] + 1;
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
			int x = current_position[0] - 1; int y = current_position[1] + 1;
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
			int x = current_position[0] + 1; int y = current_position[1] - 1;
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
			int x = current_position[0] - 1; int y = current_position[1] - 1;
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

	}
	return returnpos;
}
