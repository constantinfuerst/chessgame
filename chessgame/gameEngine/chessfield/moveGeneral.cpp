#include "pch.h"
#include "chessfield.h"

cg::move_sucess chessfield::moveCharacter(cg::position& selectedMove, move* movecounter, cg::theoretical theoretical) {
	bool possible = FALSE;
	if (theoretical != cg::onlytheoretical) {
		if (moveCasteling(selectedMove, movecounter) == cg::sucess) {
			return cg::sucess;
		}
	}
	else {
		if (moveCasteling(selectedMove, nullptr, cg::onlytheoretical) == cg::sucess) {
			return cg::sucess;
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

		if (theoretical != cg::nontheoretical) {
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
				if (theoretical != cg::nontheoretical) {
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
		if (theoretical != cg::nontheoretical) {
			movetoempty(pos, selectedMove, theoretical_field, nullptr, TRUE);
		}
		else {
			movetoempty(pos, selectedMove, theoretical_field, movecounter, TRUE);
		}
		if (check_check(col, theoretical_field) == cg::check) {
			return cg::wouldbecheck;
		}
		else if (theoretical == cg::oncetheoretical) {
			moveCharacter(selectedMove, movecounter, cg::nontheoretical);
			return cg::sucess;
		}
		else {
			return cg::sucess;
		}
	}
	else {
		return cg::impossible;
	}
}

void chessfield::movetoempty(cg::position& old_position, cg::position& new_position, chessboard* field, move* movedata, bool register_move) {
	if (old_position.x == new_position.x && old_position.y == new_position.y) {
		return;
	}
	for (auto& i : *field) {
		if (i->getPos().x == old_position.x && i->getPos().y == old_position.y) {
			if (movedata != nullptr) {
				if (register_move == TRUE) {
					movedata->makemove(i.get(), old_position, new_position, i->getHasMoved(), cg::toempty);
				}
			}
			i->setPos(new_position);
			return;
		}
	}
}

void chessfield::movetoside(cg::position& position, chessboard* virtual_field, chessboard* virtual_side, move* movedata, bool register_move) {
	for (size_t i = 0; i < virtual_field->size(); i++) {
		if (virtual_field->at(i)->getPos().x == position.x && virtual_field->at(i)->getPos().y == position.y) {
			if (movedata != nullptr) {
				const cg::position undef = { 9, 9 };
				if (register_move == TRUE) {
					movedata->makemove(virtual_field->at(i).get(), position, undef, virtual_field->at(i)->getHasMoved(), cg::toside);
				}
			}
			virtual_side->push_back(std::unique_ptr<chessmen>(virtual_field->at(i)->clone()));
			virtual_field->erase(virtual_field->begin() + i);
			return;
		}
	}
}

void chessfield::createChessmen(chessboard* chessboard, cg::chessfigure figure, cg::position pos, cg::color color, bool has_moved, move* movedata) {
	if (figure == cg::pawn)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<pawn>(color, pos, has_moved)));
	else if (figure == cg::rook)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(color, pos, has_moved)));
	else if (figure == cg::knight)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(color, pos, has_moved)));
	else if (figure == cg::bishop)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(color, pos, has_moved)));
	else if (figure == cg::queen)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(color, pos, has_moved)));
	else if (figure == cg::king)
		chessboard->push_back(std::unique_ptr<chessmen>(std::make_unique<king>(color, pos, has_moved)));
	else
		return;
	const cg::position undef = { 9, 9 };
	if (movedata != nullptr) {
		movedata->makemove(undef, pos, color, figure, TRUE, cg::newcm);
		forwardmovetrace.clear();
	}
}