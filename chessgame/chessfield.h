#pragma once

#include "pch.h"
#include "chessmen.h"
#include <iostream>

struct notfound:std::exception {
	notfound() = default;
};

class chessfield {
public:
	typedef std::vector <chessmen> chessboard;
	chessboard chessmen_onfield;
	chessboard chessmen_onside;
	chessboard selected_chessmen;
private:
	enum king_status {
		none = 0, check = 1, stale = 2, checkmate = 3
	};
	enum move_sucess {
		impossible = 0, wouldbecheck = 1, sucess = 2
	};
	enum game_status {
		end = 0, error = 1, selected = 2, running = 3
	};
	static void movetoside(chessmen::position& position, chessboard& virtual_field, chessboard& virtual_side);
	static void movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard& virtual_field);
	static king_status check_check(chessmen::color& player, chessboard& chessmen);
	king_status king_situation(chessmen::color player);
	move_sucess moveCharacter(chessmen::position& selectedMove, chessmen::color& player, bool theoretical = TRUE);
	chessmen findChessmen(chessmen::position& position);
public:
	chessfield();
	bool clickfield(chessmen::position& field, chessmen::color& player);
};

inline bool chessfield::clickfield(chessmen::position& field, chessmen::color& player) {
	if (selected_chessmen.empty()) {
		try {
			const chessmen clicked_chessmen = findChessmen(field);
			if (clicked_chessmen.player_color == player) {
				std::cout << "You can not move to your own chessmen" << std::endl;
				return error;
			}
			else {
				selected_chessmen.push_back(clicked_chessmen);
				return selected;
			}
		}
		catch (const std::exception& exception) {
			std::cout << "That field seems to be empty" << std::endl;
			return error;
		}
	}
	else {
		const auto result = moveCharacter(field, player);
		if (result != sucess) {
			if (result == wouldbecheck) {
				std::cout << "This move is not possible as it would result in you being check" << std::endl;
			}
			else if (result == impossible) {
				std::cout << "The selected figure can not move to the selected board position" << std::endl;
			}
			return error;
		}
		else {
			selected_chessmen.clear();
			if (king_situation(chessmen::black) == stale) {
				std::cout << "DRAW" << std::endl << "The black king is stale" << std::endl;
			}
			else if (king_situation(chessmen::white) == stale) {
				std::cout << "DRAW" << std::endl << "The white king is stale" << std::endl;
			}
			else if (king_situation(chessmen::white) == checkmate) {
				std::cout << "BLACK WINS" << std::endl << "The white king is mate" << std::endl;
			}
			else if (king_situation(chessmen::black) == checkmate) {
				std::cout << "WHITE WINS" << std::endl << "The black king is mate" << std::endl;
			}
			else {
				return running;
			}
			return end;
		}
	}
}

inline chessmen chessfield::findChessmen(chessmen::position& position) {
	for (auto i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i].current_position[0] == position[0] && chessmen_onfield[i].current_position[1] == position[1]) {
			return chessmen_onfield[i];
		}
	}
	throw notfound();
}

inline chessfield::move_sucess chessfield::moveCharacter(chessmen::position& selectedMove, chessmen::color& player, bool theoretical) {
	bool possible = FALSE;
	for (auto i = 0; i < selected_chessmen[0].possibleMoves(chessmen_onfield).size(); i++) {
		if (selected_chessmen[0].possibleMoves(chessmen_onfield)[i][0] == selectedMove[0] && selected_chessmen[0].possibleMoves(chessmen_onfield)[i][1] == selectedMove[1]) {
			possible = TRUE;
		}
	}
	if (possible == TRUE) {
		//prepare virtual field and set pointers to virtual or actual board depending on bool theoretical
		chessboard fieldcopy = chessmen_onfield;
		chessboard sidecopy = chessmen_onside;
		chessboard* theoretical_field;
		chessboard* theoretical_side;
		if (theoretical == TRUE) {
			theoretical_field = &fieldcopy;
			theoretical_side = &sidecopy;
		}
		else {
			theoretical_field = &chessmen_onfield;
			theoretical_side = &chessmen_onside;
		}
		//test the move
		try {
			if (findChessmen(selectedMove).player_color != player) {
				movetoside(selectedMove, *theoretical_field, *theoretical_side);
			}
		}
		catch (const std::exception& exception) {}
		movetoempty(selected_chessmen[0].current_position, selectedMove, *theoretical_field);
		if (check_check(player, *theoretical_field) == none) {
			return wouldbecheck;
		}
		else if (theoretical == TRUE) {
			moveCharacter(selectedMove, player, FALSE);
		}
		else {
			return sucess;
		}
	}
	else {
		return impossible;
	}
}

inline void chessfield::movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard& virtual_field) {
	for (auto i = 0; i < virtual_field.size(); i++) {
		if (virtual_field[i].current_position[0] == old_position[0] && virtual_field[i].current_position[1] == old_position[1]) {
			virtual_field[i].current_position[0] = new_position[0];
			virtual_field[i].current_position[1] = new_position[1];
		}
	}
}

inline void chessfield::movetoside(chessmen::position& position, chessboard& virtual_field, chessboard& virtual_side) {
	for (auto i = 0; i < virtual_field.size(); i++) {
		if (virtual_field[i].current_position[0] == position[0] && virtual_field[i].current_position[1] == position[1]) {
			virtual_side.push_back(virtual_field[i]);
			virtual_field.erase(virtual_field.begin() + i);
		}
	}
}

//not really intended for actual use, refer to
inline chessfield::king_status chessfield::check_check(chessmen::color& player, chessboard& chessmen) {
	//get the current position of the king
	chessmen::position kingpos;
	for (auto i = 0; i < chessmen.size(); i++) {
		if (chessmen[i].figure == chessmen::king && chessmen[i].player_color == player) {
			kingpos[0] = chessmen[i].current_position[0];
			kingpos[1] = chessmen[i].current_position[1];
		}
	}

	for (auto i = 0; i < chessmen.size(); i++) {
		if (chessmen[i].player_color == player) {
			std::vector <chessmen::position> possible_moves = chessmen[i].possibleMoves(chessmen);
			for (auto j = 0; j < possible_moves.size(); j++) {
				if (possible_moves[j][0] == kingpos[0] && possible_moves[j][1] == kingpos[1]) {
					return check;
				}
			}
		}
	}

	return none;
}

inline chessfield::king_status chessfield::king_situation(chessmen::color player) {
	//for every chessmen on the board
	for (auto i = 0; i < chessmen_onfield.size(); i++) {
		//if it is a friendly
		if (chessmen_onfield[i].player_color == player) {
			chessmen current_chessmen = chessmen_onfield[i];
			//for every possible move of this chessmen
			for (auto j = 0; j < current_chessmen.possibleMoves(chessmen_onfield).size(); j++) {
				//use a virtual version of the current chessboard
				chessboard theoretical_field = chessmen_onfield;
				chessboard theoretical_side;
				//execute the currently selected move on the virtual field
				chessmen::position selectedMove = current_chessmen.possibleMoves(chessmen_onfield)[j];
				try {
					if (findChessmen(selectedMove).player_color != player) {
						movetoside(selectedMove, theoretical_field, theoretical_side);
					}
					else {
						continue;
					}
				}
				catch (const std::exception& exception) {}
				movetoempty(current_chessmen.current_position, selectedMove, theoretical_field);
				//if the king is not in check with this theoretical board position, it is not checkmate
				if (check_check(player, theoretical_field) == none) {
					//return check if the king is check in the actual board position, duh
					if (check_check(player, chessmen_onfield) == check) {
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
	if (check_check(player, chessmen_onfield) == none) {
		return stale;
	}
	//else its a checkmate
	else {
		return checkmate;
	}
}

inline chessfield::chessfield() {
	//placing white chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(pawn(chessmen::white, { i, 1 }, chessmen::pawn));
		}
		//placing rooks
		chessmen_onfield.push_back(rook(chessmen::white, { 0, 0 }, chessmen::rook));
		chessmen_onfield.push_back(rook(chessmen::white, { 7, 0 }, chessmen::rook));
		//placing knights
		chessmen_onfield.push_back(knight(chessmen::white, { 1, 0 }, chessmen::knight));
		chessmen_onfield.push_back(knight(chessmen::white, { 6, 0 }, chessmen::knight));
		//placing bishops
		chessmen_onfield.push_back(bishop(chessmen::white, { 2, 0 }, chessmen::bishop));
		chessmen_onfield.push_back(bishop(chessmen::white, { 5, 0 }, chessmen::bishop));
		//placing king
		chessmen_onfield.push_back(king(chessmen::white, { 3, 0 }, chessmen::king));
		//placing queen
		chessmen_onfield.push_back(queen(chessmen::white, { 4, 0 }, chessmen::queen));
	}
	//placing black chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(pawn(chessmen::black, { i, 6 }, chessmen::pawn));
		}
		//placing rooks
		chessmen_onfield.push_back(rook(chessmen::black, { 0, 7 }, chessmen::rook));
		chessmen_onfield.push_back(rook(chessmen::black, { 7, 7 }, chessmen::rook));
		//placing knights
		chessmen_onfield.push_back(knight(chessmen::black, { 1, 7 }, chessmen::knight));
		chessmen_onfield.push_back(knight(chessmen::black, { 6, 7 }, chessmen::knight));
		//placing bishops
		chessmen_onfield.push_back(bishop(chessmen::black, { 2, 7 }, chessmen::bishop));
		chessmen_onfield.push_back(bishop(chessmen::black, { 5, 7 }, chessmen::bishop));
		//placing king
		chessmen_onfield.push_back(king(chessmen::black, { 3, 7 }, chessmen::king));
		//placing queen
		chessmen_onfield.push_back(queen(chessmen::black, { 4, 7 }, chessmen::queen));
	}
}