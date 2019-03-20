#include "pch.h"
#include "chessmen.h"

//function definitions for the pawn

chessmen* pawn::clone() const {
	return new pawn(*this);
}

cg::chessfigure pawn::figure() {
	return cg::pawn;
}

std::vector<cg::position> pawn::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	cg::position hitleft = { 0,0 };
	cg::position hitright = { 0,0 };
	cg::position onestep = { 0,0 };
	cg::position twostep = { 0,0 };

	if (player_color == cg::black) {
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

	if (board_position.y <= cg::fieldsize_y_end && board_position.y >= cg::fieldsize_y_start) {
		if (hasMoved == FALSE) {
			if (positiocheck(chessmen, onestep, player_color) == cg::empty_pos && positiocheck(chessmen, twostep, player_color) == cg::empty_pos && validpos(twostep)) {
				returnpos.push_back(twostep);
			}
		}
		if (positiocheck(chessmen, onestep, player_color) == cg::empty_pos && validpos(onestep)) {
			returnpos.push_back(onestep);
		}
		if (positiocheck(chessmen, hitleft, player_color) == cg::enemy_pos && validpos(hitleft)) {
			returnpos.push_back(hitleft);
		}
		if (positiocheck(chessmen, hitright, player_color) == cg::enemy_pos && validpos(hitright)) {
			returnpos.push_back(hitright);
		}
	}

	return returnpos;
}
