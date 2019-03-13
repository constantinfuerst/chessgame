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
	position hitleft = { 0,0 };
	position hitright = { 0,0 };
	position onestep = { 0,0 };
	position twostep = { 0,0 };

	if (player_color == black) {
		hitleft.x = board_position.x - 1;
		hitleft.y = board_position.y - 1;
		hitright.x = board_position.x + 1;
		hitright.y = board_position.y - 1;
		onestep.x = board_position.x;
		onestep.y = board_position.y - 1;
		twostep.x = board_position.x;
		twostep.y = board_position.y - 2;
	}
	else {
		hitleft.x = board_position.x - 1;
		hitleft.y = board_position.y + 1;
		hitright.x = board_position.x + 1;
		hitright.y = board_position.y + 1;
		onestep.x = board_position.x;
		onestep.y = board_position.y + 1;
		twostep.x = board_position.x;
		twostep.y = board_position.y + 2;
	}

	if (board_position.y <= fieldsize_y_end && board_position.y >= fieldsize_y_start) {
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
