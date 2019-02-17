#pragma once

#include "pch.h"
#include "chessmen.h"

struct notfound:std::exception {
	notfound() = default;
};

class chessfield {
private:
	enum king_status {
		none = 0, check = 1, checkmate = 2
	};
	std::vector <chessmen> chessmen_onfield;
	std::vector <chessmen> chessmen_onside;
	static void movetoside(chessmen::position& position, std::vector <chessmen>& virtual_field, std::vector <chessmen>& virtual_side);
	static void movetoempty(chessmen::position& old_position, chessmen::position& new_position, std::vector <chessmen>& virtual_field);
	static king_status check_check(chessmen::color& player, std::vector <chessmen>& chessmen);
	king_status check_checkmate(chessmen::color& player);
	void moveCharacter(chessmen& figure, chessmen::color& player);
	chessmen findChessmen(chessmen::position& position);
public:
	chessfield();
	void clickfield(chessmen::position& field, chessmen::color& player);
};

inline void chessfield::clickfield(chessmen::position& field, chessmen::color& player) {
	try {
		chessmen clicked_chessmen = findChessmen(field);
		if (clicked_chessmen.player_color == player) {
			return;
		}
		else {
			moveCharacter(clicked_chessmen, player);
		}
	}
	catch (const std::exception& exception) {
		return;
	}
}

inline chessmen chessfield::findChessmen(chessmen::position& position) {
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i].current_position[0] == position[0] && chessmen_onfield[i].current_position[1] == position[1]) {
			return chessmen_onfield[i];
		}
	}
	throw notfound();
}

inline void chessfield::moveCharacter(chessmen& figure, chessmen::color& player) {
	//show these moves visually
	figure.possibleMoves(chessmen_onfield);
	while (TRUE) {
		chessmen::position selectedMove;
		std::vector <chessmen> theoretical_field = chessmen_onfield;
		std::vector <chessmen> theoretical_side = chessmen_onside;
		try {
			if (findChessmen(selectedMove).player_color != player) {
				movetoside(selectedMove, theoretical_field, theoretical_side);
			}
			else {
				continue;
			}
		}
		catch (const std::exception& exception) {}
		movetoempty(figure.current_position, selectedMove, theoretical_field);
		if (check_check(player, theoretical_field) == none) {
			break;
		}
	}
}

inline void chessfield::movetoempty(chessmen::position& old_position, chessmen::position& new_position, std::vector <chessmen>& virtual_field) {
	for (size_t i = 0; i < virtual_field.size(); i++) {
		if (virtual_field[i].current_position[0] == old_position[0] && virtual_field[i].current_position[1] == old_position[1]) {
			virtual_field[i].current_position[0] = new_position[0];
			virtual_field[i].current_position[1] = new_position[1];
		}
	}
}

inline void chessfield::movetoside(chessmen::position& position, std::vector <chessmen>& virtual_field, std::vector <chessmen>& virtual_side) {
	for (size_t i = 0; i < virtual_field.size(); i++) {
		if (virtual_field[i].current_position[0] == position[0] && virtual_field[i].current_position[1] == position[1]) {
			virtual_side.push_back(virtual_field[i]);
			virtual_field.erase(virtual_field.begin() + i);
		}
	}
}

inline chessfield::king_status chessfield::check_check(chessmen::color& player, std::vector <chessmen>& chessmen) {
	//get the current position of the king
	chessmen::position kingpos;
	for (size_t i = 0; i < chessmen.size(); i++) {
		if (chessmen[i].figure == chessmen::king && chessmen[i].player_color == player) {
			kingpos[0] = chessmen[i].current_position[0];
			kingpos[1] = chessmen[i].current_position[1];
		}
	}

	for (size_t i = 0; i < chessmen.size(); i++) {
		if (chessmen[i].player_color == player) {
			std::vector <chessmen::position> possible_moves = chessmen[i].possibleMoves(chessmen);
			for (size_t j = 0; j < possible_moves.size(); j++) {
				if (possible_moves[j][0] == kingpos[0] && possible_moves[j][1] == kingpos[1]) {
					return check;
				}
			}
		}
	}

	return none;
}

inline chessfield::king_status chessfield::check_checkmate(chessmen::color& player) {
	//for every chessmen on the board
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		//if it is a friendly
		if (chessmen_onfield[i].player_color == player) {
			chessmen current_chessmen = chessmen_onfield[i];
			//for every possible move of this chessmen
			for (size_t j = 0; j < current_chessmen.possibleMoves(chessmen_onfield).size(); j++) {
				//use a virtual version of the current chessboard
				std::vector <chessmen> theoretical_field = chessmen_onfield;
				std::vector <chessmen> theoretical_side;
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
					return none;
				}
			}
		}
	}
	//if there was no not-checkmate found yet the king is checkmate
	return checkmate;
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