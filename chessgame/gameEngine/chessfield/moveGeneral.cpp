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
			if (findChessmen(selectedMove)->getPlayer() != selected_chessmen->getPlayer()) {
				if (theoretical != nontheoretical) {
					movetoside(selectedMove, theoretical_field, theoretical_side, nullptr, TRUE);
				}
				else {
					movetoside(selectedMove, theoretical_field, theoretical_side, movecounter, TRUE);
				}
			}
		}
		catch (const std::exception& exception) {}
		auto pos = selected_chessmen->getPos();
		auto col = selected_chessmen->getPlayer();
		if (theoretical != nontheoretical) {
			movetoempty(pos, selectedMove, theoretical_field, nullptr, TRUE);
		}
		else {
			movetoempty(pos, selectedMove, theoretical_field, movecounter, TRUE);
		}
		if (check_check(col, theoretical_field) == check) {
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
}

void chessfield::movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field, move* movedata, bool register_move) {
	if (old_position.x == new_position.x && old_position.y == new_position.y) {
		return;
	}
	for (auto& i : *field) {
		if (i->getPos().x == old_position.x && i->getPos().y == old_position.y) {
			if (movedata != nullptr) {
				if (register_move == TRUE) {
					movedata->makemove(i.get(), old_position, new_position, i->getHasMoved(), move::toempty);
				}
			}
			i->setPos(new_position);
			return;
		}
	}
}

void chessfield::movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side, move* movedata, bool register_move) {
	for (size_t i = 0; i < virtual_field->size(); i++) {
		if (virtual_field->at(i)->getPos().x == position.x && virtual_field->at(i)->getPos().y == position.y) {
			if (movedata != nullptr) {
				const chessmen::position undef = { 9, 9 };
				if (register_move == TRUE) {
					movedata->makemove(virtual_field->at(i).get(), position, undef, virtual_field->at(i)->getHasMoved(), move::toside);
				}
			}
			virtual_side->push_back(std::unique_ptr<chessmen>(virtual_field->at(i)->clone()));
			virtual_field->erase(virtual_field->begin() + i);
			return;
		}
	}
}

//TODO: why are there two of these? merge
void chessfield::newchessmen(chessmen::position& position, move* movedata, chessmen::color color, chessmen::chessfigure figure, bool register_move) {
	if (figure == chessmen::rook)
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(color, position, TRUE)));
	else if (figure == chessmen::knight)
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(color, position, TRUE)));
	else if (figure == chessmen::bishop)
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(color, position, TRUE)));
	else
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(color, position, TRUE)));
	const chessmen::position undef = { 9, 9 };
	if (register_move == TRUE) {
		movedata->makemove(undef, position, color, figure, TRUE, move::newcm);
		forwardmovetrace.clear();
	}
}

void chessfield::createChessmen(chessboard* chessboard, chessmen::chessfigure type, chessmen::position pos, chessmen::color colo, bool move) {
	if (type == chessmen::pawn)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<pawn>(colo, pos, move)));
	else if (type == chessmen::rook)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(colo, pos, move)));
	else if (type == chessmen::knight)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(colo, pos, move)));
	else if (type == chessmen::bishop)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(colo, pos, move)));
	else if (type == chessmen::king)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<king>(colo, pos, move)));
	else if (type == chessmen::queen)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(colo, pos, move)));
	else
		return;
}