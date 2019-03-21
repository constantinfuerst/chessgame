#include "pch.h"
#include "chessfield.h"

chessfield::chessfield() {
	current_player = cg::white;
	last_game_status = cg::next;
}

chessfield::~chessfield() {
	delete selected_chessmen;
}

chessfield::chessboard* chessfield::getField() {
	return &chessmen_onfield;
}

chessfield::chessboard* chessfield::getSide() {
	return &chessmen_onside;
}

//function called by clicking on the ui-backwardarrow
//undoes the last move
void chessfield::stepback() {
	if (!backwardmovetrace.empty()) {
		//make the move traceable (for stepforward to repeat the undone move)
		forwardmovetrace.push_back(std::make_unique<move>(*backwardmovetrace.back()));
		move movetracebackward = *backwardmovetrace.back();
		current_player = movetracebackward.current_player;
		//for every change in this move
		while (!movetracebackward.changes.empty()) {
			//get the last change and undo it
			move::chessmenMoved* move = &movetracebackward.changes.back();
			//if a new chessman was created (by pawn replacement) remove it and place a pawn at the old position
			if (move->move == cg::newcm) {
				move::removeChessmen(chessmen_onfield, move->newPosition);
				createChessmen(&chessmen_onfield, cg::pawn, move->newPosition, move->player, TRUE);
				movetracebackward.changes.pop_back(); movetracebackward.changes.pop_back();
			}
			//if it was a move to an empty field move to the old position
			else if (move->move == cg::toempty) {
				move::moveBack(chessmen_onfield, move->oldPosition, move->newPosition, move->hasmovedold);
				movetracebackward.changes.pop_back();
			}
			//if the chessmen was moved to the side move it to its original position
			else if (move->move == cg::toside) {
				move::removeChessmen(chessmen_onside, move->oldPosition, move->figure);
				createChessmen(&chessmen_onfield, move->figure, move->oldPosition, move->player, move->hasmovedold);
				movetracebackward.changes.pop_back();
			}
			//if the move is unrecognized skip
			else {
				continue;
			}
		}
		//lastly remove it from the movestack
		backwardmovetrace.pop_back();
	}
}

//function called by clicking on the ui-forwardarrow
//undoes the undo -> repeats a previously undone move
void chessfield::stepforward() {
	if (!forwardmovetrace.empty()) {
		backwardmovetrace.push_back(std::make_unique<move>(*forwardmovetrace.back()));
		move movetraceforward = *forwardmovetrace.back();
		if (movetraceforward.current_player == cg::white)
			current_player = cg::black;
		else
			current_player = cg::white;
		while (!movetraceforward.changes.empty()) {
			move::chessmenMoved* move = &movetraceforward.changes.front();
			//if a pawnreplacement happened replace the pawn again
			if (move->move == cg::newcm) {
				movetoside(move->newPosition, &chessmen_onfield, &chessmen_onside, nullptr);
				createChessmen(getField(), move->figure, move->newPosition, move->player, FALSE);
				movetraceforward.changes.erase(movetraceforward.changes.begin());
				movetraceforward.changes.erase(movetraceforward.changes.begin());
			}
			//if it was a move to an empty field just repeat that move
			else if (move->move == cg::toempty) {
				movetoempty(move->oldPosition, move->newPosition, &chessmen_onfield, nullptr);
				movetraceforward.changes.erase(movetraceforward.changes.begin());
			}
			//if it was a move to the side just repeat that
			else if (move->move == cg::toside) {
				movetoside(move->oldPosition, &chessmen_onfield, &chessmen_onside, nullptr);
				movetraceforward.changes.erase(movetraceforward.changes.begin());
			}
			//if the move is unrecognized skip
			else {
				continue;
			}
		}
		//lastly remove it from the movestack
		forwardmovetrace.pop_back();
	}
}

void chessfield::quit() {
	chessmen_onfield.clear();
	chessmen_onside.clear();
	backwardmovetrace.clear();
}

void chessfield::clean() {
	chessmen_onfield.clear();
	chessmen_onside.clear();
	backwardmovetrace.clear();
	chessmen_onfield.reserve(32);
	backwardmovetrace.reserve(32);
	forwardmovetrace.reserve(16);
	last_game_status = cg::next;
	selected_chessmen = nullptr;
}

//returns the chessmen on the main field on a specified position
chessmen* chessfield::findChessmen(cg::position& position) {
	for (auto& i : chessmen_onfield) {
		if (i->getPos().x == position.x && i->getPos().y == position.y) {
			return i.get();
		}
	}
	//throw error if not found
	throw notfound();
}

//returns the chessmen on a specified chessboard on a specified position
chessmen* chessfield::findChessmen(cg::position position, chessboard* chessboard) {
	for (auto& i : *chessboard) {
		if (i->getPos().x == position.x && i->getPos().y == position.y) {
			return i.get();
		}
	}
	//throw error if not found
	throw notfound();
}

//returns all chessmen of a type and color
std::vector<chessmen*> chessfield::findmultipleChessmen(cg::chessfigure figure, cg::color player) {
	std::vector<chessmen*> returnvec;
	for (auto& i : chessmen_onfield) {
		if (i->figure() == figure && i->getPlayer() == player) {
			returnvec.push_back(i.get());
		}
	}
	return returnvec;
}

//create a copy of a chessboard
chessfield::chessboard chessfield::copyChessboard(chessboard* chessboard_pntr) {
	chessboard returnboard;
	for (auto& i : *chessboard_pntr) {
		returnboard.push_back(std::unique_ptr<chessmen>(i->clone()));
	}
	return returnboard;
}

//loads a ".csg" file (any json file is accepted)
bool chessfield::initSaveGame(const std::string& filename) {
	//clean the gamestate
	clean();
	//load the json data from file
	nlohmann::json json;
	boost::filesystem::fstream json_input;
	json_input.open(filename);
	if (json_input.is_open()) {
		json_input >> json;
		//set the current player
		current_player = json["player"].get<cg::color>();
		//load the chessmen on the main field
		for (int i = 0; i < json["board"]["cmof"]["count"].get<int>(); i++) {
			createChessmen(
				&chessmen_onfield,
				json["board"]["cmof"]["cm" + std::to_string(i)]["type"].get<cg::chessfigure>(),
				{
					json["board"]["cmof"]["cm" + std::to_string(i)]["posx"].get<unsigned int>(),
					json["board"]["cmof"]["cm" + std::to_string(i)]["posy"].get<unsigned int>() 
				},
				json["board"]["cmof"]["cm" + std::to_string(i)]["colo"].get<cg::color>(),
				json["board"]["cmof"]["cm" + std::to_string(i)]["move"].get<bool>()
			);
		}
		//load the chessmen on the side
		for (int i = 0; i < json["board"]["cmos"]["count"].get<int>(); i++) {
			createChessmen(
				&chessmen_onside,
				json["board"]["cmos"]["cm" + std::to_string(i)]["type"].get<cg::chessfigure>(),
				{
					json["board"]["cmos"]["cm" + std::to_string(i)]["posx"].get<unsigned int>(),
					json["board"]["cmos"]["cm" + std::to_string(i)]["posy"].get<unsigned int>()
				},
				json["board"]["cmos"]["cm" + std::to_string(i)]["colo"].get<cg::color>(),
				json["board"]["cmos"]["cm" + std::to_string(i)]["move"].get<bool>()
			);
		}
		//if a chessmen was selected when saving the game select it
		if (json["board"]["cmsl"]["count"].get<int>() == 1) {
			cg::position pos = { json["board"]["cmsl"]["cm0"]["posx"].get<unsigned int>(), json["board"]["cmsl"]["cm0"]["posy"].get<unsigned int>() };
			selected_chessmen = findChessmen(pos);
		}
		//initialize the undo-moves stored
		for (int i = 0; i < json["movetracebackward"]["moves"].get<int>(); i++) {
			move movedata;
			movedata.current_player = json["movetracebackward"]["move" + std::to_string(i)]["player"].get<cg::color>();
			for (int j = 0; j < json["movetracebackward"]["move" + std::to_string(i)]["partmoves"].get<int>(); j++) {
				move::chessmenMoved changes;
				changes.figure = json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["type"].get<cg::chessfigure>();
				changes.move = json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["mvetype"].get<cg::moveType>();
				changes.player = json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["colo"].get<cg::color>();
				changes.newPosition = {
					json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposx"].get<unsigned int>(),
					json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposy"].get<unsigned int>()
				};
				changes.oldPosition = {
					json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposx"].get<unsigned int>(),
					json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposy"].get<unsigned int>()
				};
				movedata.pushmove(changes);
			}
			backwardmovetrace.push_back(std::make_unique<move>(movedata));
		}
		//initialize the undo-undo-moves stored
		for (int i = 0; i < json["movetraceforward"]["moves"].get<int>(); i++) {
			move movedata;
			movedata.current_player = json["movetraceforward"]["move" + std::to_string(i)]["player"].get<cg::color>();
			for (int j = 0; j < json["movetraceforward"]["move" + std::to_string(i)]["partmoves"].get<int>(); j++) {
				move::chessmenMoved changes;
				changes.figure = json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["type"].get<cg::chessfigure>();
				changes.move = json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["mvetype"].get<cg::moveType>();
				changes.player = json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["colo"].get<cg::color>();
				changes.newPosition = {
					json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposx"].get<unsigned int>(),
					json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposy"].get<unsigned int>()
				};
				changes.oldPosition = {
					json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposx"].get<unsigned int>(),
					json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposy"].get<unsigned int>()
				};
				movedata.pushmove(changes);
			}
			forwardmovetrace.push_back(std::make_unique<move>(movedata));
		}
		//close the json
		json_input.close();
	}
	else {
		return FALSE;
	}
	return TRUE;
}

//writes the current state of the game to a ".csg" file (any extension is accepted)
bool chessfield::createSaveGame(const std::string& filename, const std::string& gamename) {
	//open the json and filestream
	nlohmann::json json;
	boost::filesystem::ofstream json_output;
	json_output.open(filename);
	if (json_output.is_open()) {
		//save general data
		json["name"] = gamename;
		json["player"] = current_player;
		json["board"]["cmof"]["count"] = chessmen_onfield.size();
		//save chessmen on field
		for (size_t i = 0; i < chessmen_onfield.size(); i++) {
			json["board"]["cmof"]["cm" + std::to_string(i)]["type"] = chessmen_onfield[i]->figure();
			json["board"]["cmof"]["cm" + std::to_string(i)]["posx"] = chessmen_onfield[i]->getPos().x;
			json["board"]["cmof"]["cm" + std::to_string(i)]["posy"] = chessmen_onfield[i]->getPos().y;
			json["board"]["cmof"]["cm" + std::to_string(i)]["colo"] = chessmen_onfield[i]->getPlayer();
			json["board"]["cmof"]["cm" + std::to_string(i)]["move"] = chessmen_onfield[i]->getHasMoved();
		}
		//save chessmen on side
		json["board"]["cmos"]["count"] = chessmen_onside.size();
		for (size_t i = 0; i < chessmen_onside.size(); i++) {
			json["board"]["cmos"]["cm" + std::to_string(i)]["type"] = chessmen_onside[i]->figure();
			json["board"]["cmos"]["cm" + std::to_string(i)]["posx"] = chessmen_onside[i]->getPos().x;
			json["board"]["cmos"]["cm" + std::to_string(i)]["posy"] = chessmen_onside[i]->getPos().y;
			json["board"]["cmos"]["cm" + std::to_string(i)]["colo"] = chessmen_onside[i]->getPlayer();
			json["board"]["cmos"]["cm" + std::to_string(i)]["move"] = chessmen_onside[i]->getHasMoved();
		}
		//save a selected chessmen if any
		if (selected_chessmen != nullptr) {
			json["board"]["cmsl"]["count"] = 1;
			json["board"]["cmsl"]["cm0"]["type"] = selected_chessmen->figure();
			json["board"]["cmsl"]["cm0"]["posx"] = selected_chessmen->getPos().x;
			json["board"]["cmsl"]["cm0"]["posy"] = selected_chessmen->getPos().y;
			json["board"]["cmsl"]["cm0"]["colo"] = selected_chessmen->getPlayer();
			json["board"]["cmsl"]["cm0"]["move"] = selected_chessmen->getHasMoved();
		}
		//if none is selected just set the count to 0
		else {
			json["board"]["cmsl"]["count"] = 0;
		}
		//save any backwards moves
		json["movetracebackward"]["moves"] = backwardmovetrace.size();
		for (size_t i = 0; i < backwardmovetrace.size(); i++) {
			json["movetracebackward"]["move" + std::to_string(i)]["partmoves"] = backwardmovetrace[i]->changes.size();
			json["movetracebackward"]["move" + std::to_string(i)]["player"] = backwardmovetrace[i]->current_player;
			for (size_t j = 0; j < backwardmovetrace[i]->changes.size(); j++) {
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["type"] = backwardmovetrace[i]->changes[j].figure;
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["colo"] = backwardmovetrace[i]->changes[j].player;
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["mvetype"] = backwardmovetrace[i]->changes[j].move;
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposx"] = backwardmovetrace[i]->changes[j].oldPosition.x;
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposy"] = backwardmovetrace[i]->changes[j].oldPosition.y;
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposx"] = backwardmovetrace[i]->changes[j].newPosition.x;
				json["movetracebackward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposy"] = backwardmovetrace[i]->changes[j].newPosition.y;
			}
		}
		//save any forwards moves
		json["movetraceforward"]["moves"] = forwardmovetrace.size();
		for (size_t i = 0; i < forwardmovetrace.size(); i++) {
			json["movetraceforward"]["move" + std::to_string(i)]["partmoves"] = forwardmovetrace[i]->changes.size();
			json["movetraceforward"]["move" + std::to_string(i)]["player"] = forwardmovetrace[i]->current_player;
			for (size_t j = 0; j < forwardmovetrace[i]->changes.size(); j++) {
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["type"] = forwardmovetrace[i]->changes[j].figure;
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["colo"] = forwardmovetrace[i]->changes[j].player;
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["mvetype"] = forwardmovetrace[i]->changes[j].move;
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposx"] = forwardmovetrace[i]->changes[j].oldPosition.x;
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["oldposy"] = forwardmovetrace[i]->changes[j].oldPosition.y;
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposx"] = forwardmovetrace[i]->changes[j].newPosition.x;
				json["movetraceforward"]["move" + std::to_string(i)]["partmove" + std::to_string(j)]["newposy"] = forwardmovetrace[i]->changes[j].newPosition.y;
			}
		}
		//save to output file and close
		json_output << json;
		json_output.close();
	}
	else {
		return FALSE;
	}
	return TRUE;
}

//initialize a standard chessgame
void chessfield::initGame() {
	//clean the gamestate
	clean();

	//ACTUAL CHESSBOARD
	//placing white chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(
				std::unique_ptr<chessmen>(std::make_unique<pawn>(cg::white, i, cg::fieldsize_y_start + 1))
			);
		}
		//placing rooks
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::white, 0, cg::fieldsize_y_start)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::white, 7, cg::fieldsize_y_start)));
		//placing knights
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::white, 1, cg::fieldsize_y_start)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::white, 6, cg::fieldsize_y_start)));
		//placing bishops															 
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::white, 2, cg::fieldsize_y_start)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::white, 5, cg::fieldsize_y_start)));
		//placing king
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<king>(cg::white, 3, cg::fieldsize_y_start)));
		//placing queen
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(cg::white, 4, cg::fieldsize_y_start)));
	}
	//placing black chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(
				std::unique_ptr<chessmen>(std::make_unique<pawn>(cg::black, i, cg::fieldsize_y_end - 1))
			);
		}
		//placing rooks
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::black, 0, cg::fieldsize_y_end)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::black, 7, cg::fieldsize_y_end)));
		//placing knights
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::black, 1, cg::fieldsize_y_end)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::black, 6, cg::fieldsize_y_end)));
		//placing bishops															
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::black, 2, cg::fieldsize_y_end)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::black, 5, cg::fieldsize_y_end)));
		//placing king
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<king>(cg::black, 3, cg::fieldsize_y_end)));
		//placing queen
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(cg::black, 4, cg::fieldsize_y_end)));
	}
}
