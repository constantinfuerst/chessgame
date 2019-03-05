#include "pch.h"
#include "chessmen.h"

//function definitions for the queen

chessmen* queen::clone() const {
	return new queen(*this);
}

chessmen::chessfigure queen::figure() {
	return chessmen::queen;
}
std::vector<chessmen::position> queen::possibleMoves(std::vector <chessmen*>* chessmen) {
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
		{
			int x = current_position[0] + 1;
			while (x <= fieldsize_x_end) {
				const position pos = { x, current_position[1] };
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
			int x = current_position[0] - 1;
			while (x >= fieldsize_x_start) {
				const position pos = { x, current_position[1] };
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
