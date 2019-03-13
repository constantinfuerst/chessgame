#include "pch.h"
#include "chessfield.h"

chessfield::move_sucess chessfield::moveCharacter(chessmen::position& selectedMove, move* movecounter, theoretical theoretical) {
	bool possible = FALSE;
	if (theoretical != onlytheoretical) {
		if (moveCasteling(selectedMove, movecounter) == sucess) {
			return sucess;
		}
	}
	else {
		if (moveCasteling(selectedMove, nullptr, onlytheoretical) == sucess) {
			return sucess;
		}
	}
	for (size_t i = 0; i < selected_chessmen->possibleMoves(&chessmen_onfield).size(); i++) {
		if (selected_chessmen->possibleMoves(&chessmen_onfield)[i].x == selectedMove.x && selected_chessmen->possibleMoves(&chessmen_onfield)[i].y == selectedMove.y) {
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
			if (findChessmen(selectedMove)->player_color != selected_chessmen->player_color) {
				if (theoretical != nontheoretical) {
					movetoside(selectedMove, theoretical_field, theoretical_side, nullptr);
				}
				else {
					movetoside(selectedMove, theoretical_field, theoretical_side, movecounter);
				}
			}
		}
		catch (const std::exception& exception) {}
		if (theoretical != nontheoretical) {
			movetoempty(selected_chessmen->board_position, selectedMove, theoretical_field, nullptr);
		}
		else {
			movetoempty(selected_chessmen->board_position, selectedMove, theoretical_field, movecounter);
		}
		if (check_check(selected_chessmen->player_color, theoretical_field) == check) {
			return wouldbecheck;
		}
		else if (theoretical == oncetheoretical) {
			moveCharacter(selectedMove, movecounter, nontheoretical);
			return sucess;
		}
		else {
			return sucess;
		}
	}
	else {
		return impossible;
	}
	return impossible;
}

void chessfield::movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field, move* movedata) {
	if (old_position.x == new_position.x && old_position.y == new_position.y) {
		return;
	}
	for (size_t i = 0; i < field->size(); i++) {
		if (field->at(i)->board_position.x == old_position.x && field->at(i)->board_position.y == old_position.y) {
			if (movedata != nullptr) {
				movedata->makemove(field->at(i).get(), old_position, new_position, field->at(i)->hasMoved, move::toempty);
			}
			field->at(i)->board_position.x = new_position.x;
			field->at(i)->board_position.y = new_position.y;
			return;
		}
	}
	return;
}

void chessfield::movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side, move* movedata) {
	for (size_t i = 0; i < virtual_field->size(); i++) {
		if (virtual_field->at(i)->board_position.x == position.x && virtual_field->at(i)->board_position.y == position.y) {
			if (movedata != nullptr) {
				const chessmen::position undef = { 9, 9 };
				movedata->makemove(virtual_field->at(i).get(), position, undef, virtual_field->at(i)->hasMoved, move::toside);
			}
			virtual_side->push_back(std::unique_ptr<chessmen>(virtual_field->at(i)->clone()));
			virtual_field->erase(virtual_field->begin() + i);
			return;
		}
	}
	return;
}

void chessfield::newchessmen(chessmen::position& position, move* movedata, chessmen::color color, chessmen::chessfigure figure) {
	if (figure == chessmen::rook)
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new rook(color, position, TRUE)));
	else if (figure == chessmen::knight)
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new knight(color, position, TRUE)));
	else if (figure == chessmen::bishop)
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new bishop(color, position, TRUE)));
	else
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new queen(color, position, TRUE)));
	const chessmen::position undef = { 9, 9 };
	movedata->makemove(undef, position, color, figure, TRUE, move::newcm);
}