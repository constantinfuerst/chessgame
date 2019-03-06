#include "pch.h"
#include "chessfield.h"

chessfield::king_status chessfield::check_check(chessmen::color& player, chessboard* chessmen) {
	//get the current position of the king
	chessmen::position kingpos;
	for (size_t i = 0; i < chessmen->size(); i++) {
		if (chessmen->at(i)->figure() == chessmen::king && chessmen->at(i)->player_color == player) {
			kingpos[0] = chessmen->at(i)->current_position[0];
			kingpos[1] = chessmen->at(i)->current_position[1];
			break;
		}
	}

	for (size_t i = 0; i < chessmen->size(); i++) {
		if (chessmen->at(i)->player_color != player) {
			std::vector <chessmen::position> possible_moves = truePossibleMoves(chessmen->at(i).get(), chessmen, TRUE);
			for (size_t j = 0; j < possible_moves.size(); j++) {
				if (possible_moves[j][0] == kingpos[0] && possible_moves[j][1] == kingpos[1]) {
					return check;
				}
			}
		}
	}

	return none;
}

chessfield::king_status chessfield::king_situation(chessmen::color player) {
	//for every chessmen on the board
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		//if it is a friendly
		if (chessmen_onfield[i]->player_color == player) {
			chessmen* current_chessmen = chessmen_onfield[i].get();
			//for every possible move of this chessmen
			for (size_t j = 0; j < truePossibleMoves(current_chessmen, &chessmen_onfield).size(); j++) {
				//use a virtual version of the current chessboard
				chessboard theoretical_field = copyChessboard(&chessmen_onfield);
				chessboard theoretical_side;
				//execute the currently selected move on the virtual field
				chessmen::position selectedMove = truePossibleMoves(current_chessmen, &chessmen_onfield)[j];
				try {
					if (findChessmen(selectedMove)->player_color != player) {
						movetoside(selectedMove, &theoretical_field, &theoretical_side);
					}
					else {
						continue;
					}
				}
				catch (const std::exception& exception) {}
				movetoempty(current_chessmen->current_position, selectedMove, &theoretical_field);
				//if the king is not in check with this theoretical board position, it is not checkmate
				if (check_check(player, &theoretical_field) == none) {
					//return check if the king is check in the actual board position, duh
					if (check_check(player, &chessmen_onfield) == check) {
						return check;
					}
					//if not return none
					else {
						return none;
					}
				}
			}
		}
	}
	//if no check-free found the king is mate
	//if the king is currently not in check then its a stale
	if (check_check(player, &chessmen_onfield) == none) {
		return stale;
	}
	//else its a checkmate
	else {
		return checkmate;
	}
}