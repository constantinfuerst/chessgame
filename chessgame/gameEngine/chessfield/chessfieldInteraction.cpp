#include "pch.h"
#include "chessfield.h"

std::vector<chessmen::position> chessfield::truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate) {
	std::vector<chessmen::position> returnpos = chessmen->possibleMoves(chessboard);
	bool clear = FALSE;

	if (dontCheckMate != TRUE) {
		if (selected_chessmen.empty()) {
			clear = TRUE;
			selected_chessmen.push_back(chessmen);
		}

		size_t size = returnpos.size();
		size_t count = 0;
		while (count < size) {
			if (moveCharacter(returnpos[count], onlytheoretical) == wouldbecheck) {
				returnpos.erase(returnpos.begin() + count);
				size = returnpos.size();
			}
			else {
				count++;
			}
		}

		if (clear == TRUE) {
			selected_chessmen.clear();
		}

		if (chessmen->figure() == chessmen::king) {
			for (size_t i = 0; i < casteling(chessmen->player_color).size(); i++) {
				returnpos.push_back(std::get<0>(casteling(chessmen->player_color)[i]));
			}
		}
	}

	return returnpos;
}

chessfield::full_game_status chessfield::clickfield(chessmen::position field, chessmen::color player) {
	if (selected_chessmen.empty()) {
		try {
			chessmen* clicked_chessmen = findChessmen(field);
			if (clicked_chessmen->player_color != player) {
				return enemy;
			}
			else {
				selected_chessmen.push_back(clicked_chessmen);
				return selected;
			}
		}
		catch (const std::exception& exception) {
			return emptyfield;
		}
	}
	else {
		const auto result = moveCharacter(field, oncetheoretical);
		if (result != sucess) {
			if (result == wouldbecheck) {
				return checked;
			}
			else if (result == impossible) {
				return impmove;
			}
		}
		else {
			selected_chessmen[0]->hasMoved = TRUE;
			//replacing pawn at the end of field with queen
			if (selected_chessmen[0]->figure() == chessmen::pawn) {
				if (selected_chessmen[0]->current_position[1] == chessmen::fieldsize_y_start && selected_chessmen[0]->player_color == chessmen::black || selected_chessmen[0]->current_position[1] == chessmen::fieldsize_y_end && selected_chessmen[0]->player_color == chessmen::white) {
					const chessmen::position pos = selected_chessmen[0]->current_position;
					const chessmen::color col = selected_chessmen[0]->player_color;
					movetoside(selected_chessmen[0]->current_position, &chessmen_onfield, &chessmen_onside);
					chessmen_onfield.push_back(new queen(col, pos));
				}
			}
			selected_chessmen.clear();
			if (king_situation(chessmen::black) == stale) {
				return bkstale;
			}
			else if (king_situation(chessmen::white) == stale) {
				return wkstale;
			}
			else if (king_situation(chessmen::white) == checkmate) {
				return wkmate;
			}
			else if (king_situation(chessmen::black) == checkmate) {
				return bkmate;
			}
			else {
				return next;
			}
		}
	}
	return error;
}
