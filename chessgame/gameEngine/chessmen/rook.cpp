#include "pch.h"
#include "chessmen.h"

//function definitions for the rook

chessmen* rook::clone() const {
	return new rook(*this);
}

chessmen::chessfigure rook::figure() {
	return chessmen::rook;
}

std::vector<chessmen::position> rook::possibleMoves(chessboard* chessmen) {
	std::vector<position> returnpos;
	{
		{
			int x = current_position[0] + 1;
			while (x <= fieldsize_x_end) {
				const position pos = { x, current_position[1] };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
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
			int x = current_position[0] - 1;
			while (x >= fieldsize_x_start) {
				const position pos = { x, current_position[1] };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
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
			int y = current_position[1] + 1;
			while (y <= fieldsize_y_end) {
				const position pos = { current_position[0], y };
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
			int y = current_position[1] - 1;
			while (y >= fieldsize_y_start) {
				const position pos = { current_position[0], y };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
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
