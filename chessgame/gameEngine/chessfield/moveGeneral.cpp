#include "pch.h"
#include "chessfield.h"

//proxy between the actual move functions, can be used with a clone of onfield (-> theoretical) or to actually execute a move
cg::move_sucess chessfield::moveCharacter(cg::position& selectedMove, move* movecounter, cg::theoretical theoretical) {
	bool possible = FALSE;
	//if it is not just theoretical
	if (theoretical != cg::onlytheoretical) {
		//check if casteling would work and if so return true
		if (moveCasteling(selectedMove, movecounter) == cg::sucess) {
			return cg::sucess;
		}
	}
	//we are executing only theoretically
	else {
		//check if casteling would work theoretically and return
		if (moveCasteling(selectedMove, nullptr, cg::onlytheoretical) == cg::sucess) {
			return cg::sucess;
		}
	}
	//for every move of the selected chessmen
	for (size_t i = 0; i < selected_chessmen->possibleMoves(&chessmen_onfield).size(); i++) {
		//if a move matching selectedMove then the entered move is possible
		if (selected_chessmen->possibleMoves(&chessmen_onfield)[i].x == selectedMove.x && selected_chessmen->possibleMoves(&chessmen_onfield)[i].y == selectedMove.y) {
			possible = TRUE;
			break;
		}
	}
	//if we can execute the move
	if (possible == TRUE) {
		//prepare virtual field and set pointers to virtual or actual board depending on bool theoretical
		chessboard fieldcopy;
		chessboard sidecopy;

		chessboard* theoretical_field;
		chessboard* theoretical_side;

		//if we are in a theoretical state
		if (theoretical != cg::nontheoretical) {
			//copy the current fields and set the pointers to that copy
			fieldcopy = copyChessboard(&chessmen_onfield);
			sidecopy = copyChessboard(&chessmen_onside);
			theoretical_field = &fieldcopy;
			theoretical_side = &sidecopy;
		}
		//if we are actually executing the move
		else {
			//set the pointers to the actual fields
			theoretical_field = &chessmen_onfield;
			theoretical_side = &chessmen_onside;
		}
		//if there is a chessmen found on the destination (selectedMove) then move that chessmen to side
		try {
			if (findChessmen(selectedMove)->getPlayer() != selected_chessmen->getPlayer()) {
				if (theoretical != cg::nontheoretical) {
					movetoside(selectedMove, theoretical_field, theoretical_side, nullptr);
				}
				else {
					movetoside(selectedMove, theoretical_field, theoretical_side, movecounter);
				}
			}
		}
		catch (const std::exception& exception) {}
		auto pos = selected_chessmen->getPos();
		auto col = selected_chessmen->getPlayer();
		//if we are theoretical execute the move theoretically
		if (theoretical != cg::nontheoretical) {
			movetoempty(pos, selectedMove, theoretical_field, nullptr);
		}
		//if not the execute it actually
		else {
			movetoempty(pos, selectedMove, theoretical_field, movecounter);
		}
		//now check whether we would be check with that move
		if (check_check(col, theoretical_field) == cg::check) {
			//we would be check -> return that
			return cg::wouldbecheck;
		}
		//if we were just checking for the move being valid then execute the valid move now
		else if (theoretical == cg::oncetheoretical) {
			moveCharacter(selectedMove, movecounter, cg::nontheoretical);
			return cg::sucess;
		}
		else {
			return cg::sucess;
		}
	}
	//if we cant execute the move it is impossible, return
	else {
		return cg::impossible;
	}
}

//function expecting a empty new position and a old position with a chessmen that can move to the new position
void chessfield::movetoempty(cg::position& old_position, cg::position& new_position, chessboard* field, move* movedata) {
	if (old_position.x == new_position.x && old_position.y == new_position.y) {
		return;
	}
	for (auto& i : *field) {
		if (i->getPos().x == old_position.x && i->getPos().y == old_position.y) {
			if (movedata != nullptr) {
				movedata->makemove(i.get(), old_position, new_position, i->getHasMoved(), cg::toempty);
			}
			i->setPos(new_position);
			return;
		}
	}
}

//function expecting position with a chessmen on it, moves the chessmen to the side
void chessfield::movetoside(cg::position& position, chessboard* virtual_field, chessboard* virtual_side, move* movedata) {
	for (size_t i = 0; i < virtual_field->size(); i++) {
		if (virtual_field->at(i)->getPos().x == position.x && virtual_field->at(i)->getPos().y == position.y) {
			if (movedata != nullptr) {
				const cg::position undef = { 9, 9 };
				movedata->makemove(virtual_field->at(i).get(), position, undef, virtual_field->at(i)->getHasMoved(), cg::toside);
			}
			virtual_side->push_back(std::unique_ptr<chessmen>(virtual_field->at(i)->clone()));
			virtual_field->erase(virtual_field->begin() + i);
			return;
		}
	}
}

//function creating a chessmen on a specified position
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