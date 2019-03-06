#include "pch.h"
#include "chessmen.h"

//function definitions for the pawn

chessmen* pawn::clone() const {
	return new pawn(*this);
}

chessmen::chessfigure pawn::figure() {
	return chessmen::pawn;
}

std::vector<chessmen::position> pawn::possibleMoves(chessboard* chessmen) {
	std::vector<position> returnpos;
	position hitleft;
	position hitright;
	position onestep;
	position twostep;

	if (player_color == black) {
		hitleft[0] = current_position[0] - 1;
		hitleft[1] = current_position[1] - 1;
		hitright[0] = current_position[0] + 1;
		hitright[1] = current_position[1] - 1;
		onestep[0] = current_position[0];
		onestep[1] = current_position[1] - 1;
		twostep[0] = current_position[0];
		twostep[1] = current_position[1] - 2;
	}
	else if (player_color == white) {
		hitleft[0] = current_position[0] - 1;
		hitleft[1] = current_position[1] + 1;
		hitright[0] = current_position[0] + 1;
		hitright[1] = current_position[1] + 1;
		onestep[0] = current_position[0];
		onestep[1] = current_position[1] + 1;
		twostep[0] = current_position[0];
		twostep[1] = current_position[1] + 2;
	}

	if (current_position[1] <= fieldsize_y_end && current_position[1] >= fieldsize_y_start) {
		if (hasMoved == FALSE) {
			if (positiocheck(chessmen, onestep, player_color) == empty && positiocheck(chessmen, twostep, player_color) == empty && validpos(twostep)) {
				returnpos.push_back(twostep);
			}
		}
		if (positiocheck(chessmen, onestep, player_color) == empty && validpos(onestep)) {
			returnpos.push_back(onestep);
		}
		if (positiocheck(chessmen, hitleft, player_color) == enemy && validpos(hitleft)) {
			returnpos.push_back(hitleft);
		}
		if (positiocheck(chessmen, hitright, player_color) == enemy && validpos(hitright)) {
			returnpos.push_back(hitright);
		}
	}

	return returnpos;
}
