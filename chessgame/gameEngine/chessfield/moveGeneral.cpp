#include "pch.h"
#include "chessfield.h"

chessfield::move_sucess chessfield::moveCharacter(chessmen::position& selectedMove, theoretical theoretical) {
	bool possible = FALSE;
	if (theoretical != onlytheoretical) {
		if (moveCasteling(selectedMove) == sucess) {
			return sucess;
		}
	}
	else {
		if (moveCasteling(selectedMove, onlytheoretical) == sucess) {
			return sucess;
		}
	}
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

void chessfield::movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field) {
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

void chessfield::movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side) {
	for (size_t i = 0; i < virtual_field->size(); i++) {
		if (virtual_field->at(i)->current_position[0] == position[0] && virtual_field->at(i)->current_position[1] == position[1]) {
			virtual_side->push_back(virtual_field->at(i));
			virtual_field->erase(virtual_field->begin() + i);
			return;
		}
	}
}
