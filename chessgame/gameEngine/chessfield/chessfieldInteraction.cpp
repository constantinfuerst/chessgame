#include "pch.h"

#include "chessfield.h"

//used to combine any "special moves" (displays castling if possible)
std::vector<cg::position> chessfield::
truePossibleMoves(chessmen* chessmen, chessboard* chessboard, const bool dontCheckMate) {
	//create the return-vector and fill with all possible moves for the chessmen
	std::vector<cg::position> returnpos = chessmen->possibleMoves(chessboard);
	bool clear = FALSE;
	if (dontCheckMate != TRUE) {
		//if no selected chessmen, select the entered one
		if (selected_chessmen == nullptr) {
			clear = TRUE;
			selected_chessmen = chessmen;
		}

		//erase the impossible moves from the vector
		size_t size = returnpos.size();
		size_t count = 0;
		while (count < size) {
			if (moveCharacter(returnpos[count], nullptr, cg::onlytheoretical) == cg::wouldbecheck) {
				returnpos.erase(returnpos.begin() + count);
				size = returnpos.size();
			}
			else {
				count++;
			}
		}

		if (selected_chessmen->figure() == cg::king) {
			auto casteling_pos = casteling(selected_chessmen->getPlayer());
			for (auto& casteling_po : casteling_pos) {
				returnpos.push_back(casteling_po.newkpos);
			}
		}

		//if we previously added a new chessmen to selected unselect
		if (clear == TRUE) {
			selected_chessmen = nullptr;
		}
	}
	return returnpos;
}

//the main function that handles click input by the user with the GUI being a proxy
cg::full_game_status chessfield::clickfield(cg::position field, const cg::color player) {
	if (last_game_status >= 8) {
		//when the game is already over return the last state
		return last_game_status;
	}
	//if no chessmen is selected try to select one on the clicked position
	if (selected_chessmen == nullptr) {
		//try to find a chessmen on the clicked position and if found select it
		try {
			chessmen* clicked_chessmen = findChessmen(field);
			if (clicked_chessmen->getPlayer() != player) {
				//an enemy was clicked
				return cg::enemy;
			}
			selected_chessmen = clicked_chessmen;
			//a valid chessmen was selected
			return cg::selected;
		}
		catch (const std::exception& exception) {
			//an empty field was clicked
			return cg::emptyfield;
		}
	}
	//if a chessmen is already selected
	//prepare the move to be put on traceback
	move changes;
	changes.current_player = current_player;
	//make the move with the selected position and handle output with "result"
	const auto result = moveCharacter(field, &changes, cg::oncetheoretical);
	//if the move didn't successfully complete check what happened
	if (result != cg::sucess) {
		//if the move would result in check return "check"
		if (result == cg::wouldbecheck) {
			return cg::checked;
		}
		//if the move is impossible return "impmove"
		if (result == cg::impossible) {
			return cg::impmove;
		}
	}
		//if the move completed successfully
	else {
		//set the "hasmoved" state of the selected chessmen to TRUE
		selected_chessmen->setHasMoved(TRUE);
		//replacing pawn at the end of field with queen
		if (selected_chessmen->figure() == cg::pawn) {
			//if the pawn is at the end of the field respectively
			if (selected_chessmen->getPos().y == cg::fieldsize_y_start && selected_chessmen->getPlayer() == cg::black ||
				selected_chessmen->getPos().y == cg::fieldsize_y_end && selected_chessmen->getPlayer() == cg::white) {
				//do all the replacing
				cg::position pos = selected_chessmen->getPos();
				const cg::color col = selected_chessmen->getPlayer();
				movetoside(pos, &chessmen_onfield, &chessmen_onside, &changes);
				createChessmen(getField(), cg::queen, pos, col, TRUE);
			}
		}
		//unselect the chessmen now and clear the forwardmovetrace (as it is inaccessible now)
		selected_chessmen = nullptr;
		forwardmovetrace.clear();
		backwardmovetrace.push_back(std::make_unique<move>(changes));
		//check the state of the king now
		if (king_situation(cg::black) == cg::stale) {
			//if the black king is stale return so
			last_game_status = cg::bkstale;
			return cg::bkstale;
		}
		if (king_situation(cg::white) == cg::stale) {
			//if the white king is stale return so
			last_game_status = cg::wkstale;
			return cg::wkstale;
		}
		if (king_situation(cg::white) == cg::checkmate) {
			//if the white king is mate return so
			last_game_status = cg::wkmate;
			return cg::wkmate;
		}
		if (king_situation(cg::black) == cg::checkmate) {
			//if the black king is mate return so
			last_game_status = cg::bkmate;
			return cg::bkmate;
		}
		//if no game-end is reached return next
		return cg::next;
	}
	//if control flow end was reached without returning a valid state return an error
	return cg::error;
}
