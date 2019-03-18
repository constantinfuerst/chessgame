#include "pch.h"
#include "chessfield.h"

chessfield::king_status chessfield::check_check(chessmen::color& player, chessboard* chessmen) {
	//get the current position of the king
	std::unique_ptr<chessmen::position> kingpos = nullptr;
	for (auto& i : *chessmen) {
		if (i->figure() == chessmen::king && i->getPlayer() == player) {
			kingpos = std::make_unique<chessmen::position>(i->getPos());
			break;
		}
	}
	if (kingpos != nullptr) {
		for (size_t i = 0; i < chessmen->size(); i++) {
			if (chessmen->at(i)->getPlayer() != player) {
				std::vector <chessmen::position> possible_moves = truePossibleMoves(chessmen->at(i).get(), chessmen, TRUE);
				for (auto& possible_move : possible_moves) {
					if (possible_move.x == kingpos->x && possible_move.y == kingpos->y) {
						return check;
					}
				}
			}
		}
	}
	return none;
}

chessfield::king_status chessfield::king_situation(chessmen::color player) {
	//for every chessmen on the board
	for (auto& i : chessmen_onfield) {
		//if it is a friendly
		if (i->getPlayer() == player) {
			chessmen* current_chessmen = i.get();
			//for every possible move of this chessmen
			for (size_t j = 0; j < truePossibleMoves(current_chessmen, &chessmen_onfield).size(); j++) {
				//use a virtual version of the current chessboard
				chessboard theoretical_field = copyChessboard(&chessmen_onfield);
				chessboard theoretical_side;
				//execute the currently selected move on the virtual field
				chessmen::position selectedMove = truePossibleMoves(current_chessmen, &chessmen_onfield)[j];
				try {
					if (findChessmen(selectedMove)->getPlayer() != player) {
						movetoside(selectedMove, &theoretical_field, &theoretical_side, nullptr, FALSE);
					}
					else {
						continue;
					}
				}
				catch (const std::exception& exception) {}
				auto pos = current_chessmen->getPos();
				movetoempty(pos, selectedMove, &theoretical_field, nullptr, FALSE);
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
