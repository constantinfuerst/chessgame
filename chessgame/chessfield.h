#pragma once

#include "pch.h"
#include "chessmen.h"

struct notfound:std::exception {
	notfound() = default;
};

class chessfield {
public:
	typedef std::vector <chessmen*> chessboard;
	chessboard chessmen_onfield;
	chessboard chessmen_onside;
	chessboard selected_chessmen;
	enum full_game_status {
		error, enemy, selected, next, ownchessmen, emptyfield, checked, impmove, bkstale, wkstale, bkmate, wkmate
	};
	enum game_status {
		end, running, mistake
	};
private:
	enum king_status {
		none = 0, check = 1, stale = 2, checkmate = 3
	};
	enum move_sucess {
		impossible = 0, wouldbecheck = 1, sucess = 2
	};
	enum theoretical {
		nontheoretical = 0, oncetheoretical = 1, onlytheoretical = 2
	};
	enum specialmovetype {
		casteling_type = 0, pawnreplace_type = 1
	};
	typedef std::tuple<chessmen::position, specialmovetype> specialmovetpl;
	static chessboard copyChessboard(chessboard* chessboard_pntr);
	static void movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side);
	static void movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field);
	king_status check_check(chessmen::color& player, chessboard* chessmen);
	king_status king_situation(chessmen::color player);
	move_sucess moveCharacter(chessmen::position& selectedMove, theoretical theoretical = oncetheoretical);
	chessmen* findChessmen(chessmen::position& position);
public:
	//this is what the player or gameloop interacts with
	full_game_status clickfield(chessmen::position field, chessmen::color player);
	void initGame();
	//this is what the renderer uses to display legal moves
	std::vector<chessmen::position> truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate = FALSE);
};

inline std::vector<chessmen::position> chessfield::truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate) {
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
	}

	//TODO: call casteling

	return returnpos;
}

inline chessfield::full_game_status chessfield::clickfield(chessmen::position field, chessmen::color player) {
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

inline chessmen* chessfield::findChessmen(chessmen::position& position) {
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->current_position[0] == position[0] && chessmen_onfield[i]->current_position[1] == position[1]) {
			return chessmen_onfield[i];
		}
	}
	throw notfound();
}

inline chessfield::move_sucess chessfield::moveCharacter(chessmen::position& selectedMove, theoretical theoretical) {
	bool possible = FALSE;
	for (size_t i = 0; i < selected_chessmen[0]->possibleMoves(&chessmen_onfield).size(); i++) {
		if (selected_chessmen[0]->possibleMoves(&chessmen_onfield)[i][0] == selectedMove[0] && selected_chessmen[0]->possibleMoves(&chessmen_onfield)[i][1] == selectedMove[1]) {
			possible = TRUE;
			break;
		}
	}
	if (possible == TRUE) {
		//prepare virtual field and set pointers to virtual or actual board depending on bool theoretical
		chessboard fieldcopy = copyChessboard(&chessmen_onfield);
		chessboard sidecopy = copyChessboard(&chessmen_onside);
		chessboard* theoretical_field;
		chessboard* theoretical_side;
		if (theoretical != nontheoretical) {
			theoretical_field = &fieldcopy;
			theoretical_side = &sidecopy;
		}
		else {
			theoretical_field = &chessmen_onfield;
			theoretical_side = &chessmen_onside;
		}
		//test the move
		try {
			if (findChessmen(selectedMove)->player_color != selected_chessmen[0]->player_color) {
				movetoside(selectedMove, theoretical_field, theoretical_side);
			}
		}
		catch (const std::exception& exception) {}
		movetoempty(selected_chessmen[0]->current_position, selectedMove, theoretical_field);
		if (check_check(selected_chessmen[0]->player_color, theoretical_field) == check) {
			return wouldbecheck;
		}
		else if (theoretical == oncetheoretical) {
			moveCharacter(selectedMove, nontheoretical);
			return sucess;
		}
		else {
			return sucess;
		}
	}
	else {
		return impossible;
	}
}

inline void chessfield::movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field) {
	if (old_position[0] == new_position[0] && old_position[1] == new_position[1]) {
		return;
	}
	for (size_t i = 0; i < field->size(); i++) {
		if (field->at(i)->current_position[0] == old_position[0] && field->at(i)->current_position[1] == old_position[1]) {
			field->at(i)->current_position[0] = new_position[0];
			field->at(i)->current_position[1] = new_position[1];
			return;
		}
	}
}

inline chessfield::chessboard chessfield::copyChessboard(chessboard* chessboard_pntr) {
	chessboard returnboard;
	for (size_t i = 0; i < chessboard_pntr->size(); i++) {
		returnboard.push_back(chessboard_pntr->at(i)->clone());
	}
	return returnboard;
}

inline void chessfield::movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side) {
	for (size_t i = 0; i < virtual_field->size(); i++) {
		if (virtual_field->at(i)->current_position[0] == position[0] && virtual_field->at(i)->current_position[1] == position[1]) {
			virtual_side->push_back(virtual_field->at(i));
			virtual_field->erase(virtual_field->begin() + i);
			return;
		}
	}
}

inline chessfield::king_status chessfield::check_check(chessmen::color& player, chessboard* chessmen) {
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
			std::vector <chessmen::position> possible_moves = truePossibleMoves(chessmen->at(i), chessmen, TRUE);
			for (size_t j = 0; j < possible_moves.size(); j++) {
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
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		//if it is a friendly
		if (chessmen_onfield[i]->player_color == player) {
			chessmen* current_chessmen = chessmen_onfield[i];
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

inline void chessfield::initGame() {
	chessmen_onfield.clear();
	chessmen_onside.clear();
	selected_chessmen.clear();

	chessmen_onfield.push_back(new king(chessmen::white, { 3, chessmen::fieldsize_y_start }));
	chessmen_onfield.push_back(new rook(chessmen::white, { 0, chessmen::fieldsize_y_start }));
	chessmen_onfield.push_back(new rook(chessmen::white, { 7, chessmen::fieldsize_y_start }));
	chessmen_onfield.push_back(new pawn(chessmen::white, { 0, chessmen::fieldsize_y_end - 1}));
	chessmen_onfield.push_back(new pawn(chessmen::white, { 1, chessmen::fieldsize_y_end - 1 }));
	chessmen_onfield.push_back(new pawn(chessmen::black, { 2, chessmen::fieldsize_y_end }));
	chessmen_onfield.push_back(new king(chessmen::black, { 3, chessmen::fieldsize_y_end }));

	//ACTUAL CHESSBOARD
	//placing white chessmen
	/*
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(new pawn(chessmen::white, { i, chessmen::fieldsize_y_start + 1 }));
		}
		//placing rooks
		chessmen_onfield.push_back(new rook(chessmen::white, { 0, chessmen::fieldsize_y_start }));
		chessmen_onfield.push_back(new rook(chessmen::white, { 7, chessmen::fieldsize_y_start }));
		//placing knights
		chessmen_onfield.push_back(new knight(chessmen::white, { 1, chessmen::fieldsize_y_start }));
		chessmen_onfield.push_back(new knight(chessmen::white, { 6, chessmen::fieldsize_y_start }));
		//placing bishops
		chessmen_onfield.push_back(new bishop(chessmen::white, { 2, chessmen::fieldsize_y_start }));
		chessmen_onfield.push_back(new bishop(chessmen::white, { 5, chessmen::fieldsize_y_start }));
		//placing king
		chessmen_onfield.push_back(new king(chessmen::white, { 3, chessmen::fieldsize_y_start }));
		//placing queen
		chessmen_onfield.push_back(new queen(chessmen::white, { 4, chessmen::fieldsize_y_start }));
	}
	//placing black chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(new pawn(chessmen::black, { i, chessmen::fieldsize_y_end - 1 }));
		}
		//placing rooks
		chessmen_onfield.push_back(new rook(chessmen::black, { 0, chessmen::fieldsize_y_end }));
		chessmen_onfield.push_back(new rook(chessmen::black, { 7, chessmen::fieldsize_y_end }));
		//placing knights
		chessmen_onfield.push_back(new knight(chessmen::black, { 1, chessmen::fieldsize_y_end }));
		chessmen_onfield.push_back(new knight(chessmen::black, { 6, chessmen::fieldsize_y_end }));
		//placing bishops
		chessmen_onfield.push_back(new bishop(chessmen::black, { 2, chessmen::fieldsize_y_end }));
		chessmen_onfield.push_back(new bishop(chessmen::black, { 5, chessmen::fieldsize_y_end }));
		//placing king
		chessmen_onfield.push_back(new king(chessmen::black, { 3, chessmen::fieldsize_y_end }));
		//placing queen
		chessmen_onfield.push_back(new queen(chessmen::black, { 4, chessmen::fieldsize_y_end }));
	}
	*/
}