#include "pch.h"
#include "chessfield.h"
#include "../cg_defs.h"
#include <iomanip>

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

void chessfield::stepback() {
	if (!backwardmovetrace.empty()) {
		forwardmovetrace.push_back(std::make_unique<move>(*backwardmovetrace.back()));
		move movetracebackward = *backwardmovetrace.back();
		current_player = movetracebackward.current_player;
		while (!movetracebackward.changes.empty()) {
			move::chessmenMoved* move = &movetracebackward.changes.back();
			if (move->move == cg::newcm) {
				move::removeChessmen(chessmen_onfield, move->newPosition);
				createChessmen(&chessmen_onfield, cg::pawn, move->newPosition, move->player, TRUE);
				movetracebackward.changes.pop_back(); movetracebackward.changes.pop_back();
			}
			else if (move->move == cg::toempty) {
				move::moveBack(chessmen_onfield, move->oldPosition, move->newPosition, move->hasmovedold);
				movetracebackward.changes.pop_back();
			}
			else if (move->move == cg::toside) {
				move::removeChessmen(chessmen_onside, move->oldPosition, move->figure);
				createChessmen(&chessmen_onfield, move->figure, move->oldPosition, move->player, move->hasmovedold);
				movetracebackward.changes.pop_back();
			}
			else {
				continue;
			}
		}
		backwardmovetrace.pop_back();
	}
}

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
			if (move->move == cg::newcm) {
				movetoside(move->newPosition, &chessmen_onfield, &chessmen_onside, nullptr, FALSE);
				createChessmen(getField(), move->figure, move->newPosition, move->player, FALSE);
				movetraceforward.changes.erase(movetraceforward.changes.begin());
				movetraceforward.changes.erase(movetraceforward.changes.begin());
			}
			else if (move->move == cg::toempty) {
				movetoempty(move->oldPosition, move->newPosition, &chessmen_onfield, nullptr, FALSE);
				movetraceforward.changes.erase(movetraceforward.changes.begin());
			}
			else if (move->move == cg::toside) {
				movetoside(move->oldPosition, &chessmen_onfield, &chessmen_onside, nullptr, FALSE);
				movetraceforward.changes.erase(movetraceforward.changes.begin());
			}
			else {
				continue;
			}
		}
		forwardmovetrace.pop_back();
	}
}

void chessfield::quit() {
	delete selected_chessmen;
	chessmen_onfield.clear();
	chessmen_onside.clear();
	backwardmovetrace.clear();
}

chessmen* chessfield::findChessmen(cg::position& position) {
	for (auto& i : chessmen_onfield) {
		if (i->getPos().x == position.x && i->getPos().y == position.y) {
			return i.get();
		}
	}
	throw notfound();
}

chessmen* chessfield::findChessmen(cg::position position, chessboard* chessboard) {
	for (auto& i : *chessboard) {
		if (i->getPos().x == position.x && i->getPos().y == position.y) {
			return i.get();
		}
	}
	throw notfound();
}

chessfield::chessboard chessfield::copyChessboard(chessboard* chessboard_pntr) {
	chessboard returnboard;
	for (auto& i : *chessboard_pntr) {
		returnboard.push_back(std::unique_ptr<chessmen>(i->clone()));
	}
	return returnboard;
}

bool chessfield::initSaveGame(const std::string& filename) {
	last_game_status = cg::next;
	selected_chessmen = nullptr;
	chessmen_onfield.clear();
	chessmen_onside.clear();
	nlohmann::json json;
	boost::filesystem::fstream json_input;
	json_input.open(filename);
	if (json_input.is_open()) {
		json_input >> json;
		current_player = json["player"].get<cg::color>();
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
		if (json["board"]["cmsl"]["count"].get<int>() == 1) {
			cg::position pos = { json["board"]["cmsl"]["cm0"]["posx"].get<unsigned int>(), json["board"]["cmsl"]["cm0"]["posy"].get<unsigned int>() };
			selected_chessmen = findChessmen(pos);
		}
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
		
		json_input.close();
	}
	else {
		return FALSE;
	}
	return TRUE;
}

bool chessfield::createSaveGame(const std::string& filename, const std::string& gamename) {
	nlohmann::json json;
	boost::filesystem::ofstream json_output;
	json_output.open(filename);
	if (json_output.is_open()) {
		json["name"] = gamename;
		json["player"] = current_player;
		json["board"]["cmof"]["count"] = chessmen_onfield.size();
		for (size_t i = 0; i < chessmen_onfield.size(); i++) {
			json["board"]["cmof"]["cm" + std::to_string(i)]["type"] = chessmen_onfield[i]->figure();
			json["board"]["cmof"]["cm" + std::to_string(i)]["posx"] = chessmen_onfield[i]->getPos().x;
			json["board"]["cmof"]["cm" + std::to_string(i)]["posy"] = chessmen_onfield[i]->getPos().y;
			json["board"]["cmof"]["cm" + std::to_string(i)]["colo"] = chessmen_onfield[i]->getPlayer();
			json["board"]["cmof"]["cm" + std::to_string(i)]["move"] = chessmen_onfield[i]->getHasMoved();
		}
		json["board"]["cmos"]["count"] = chessmen_onside.size();
		for (size_t i = 0; i < chessmen_onside.size(); i++) {
			json["board"]["cmos"]["cm" + std::to_string(i)]["type"] = chessmen_onside[i]->figure();
			json["board"]["cmos"]["cm" + std::to_string(i)]["posx"] = chessmen_onside[i]->getPos().x;
			json["board"]["cmos"]["cm" + std::to_string(i)]["posy"] = chessmen_onside[i]->getPos().y;
			json["board"]["cmos"]["cm" + std::to_string(i)]["colo"] = chessmen_onside[i]->getPlayer();
			json["board"]["cmos"]["cm" + std::to_string(i)]["move"] = chessmen_onside[i]->getHasMoved();
		}
		if (selected_chessmen != nullptr) {
			json["board"]["cmsl"]["count"] = 1;
			json["board"]["cmsl"]["cm0"]["type"] = selected_chessmen->figure();
			json["board"]["cmsl"]["cm0"]["posx"] = selected_chessmen->getPos().x;
			json["board"]["cmsl"]["cm0"]["posy"] = selected_chessmen->getPos().y;
			json["board"]["cmsl"]["cm0"]["colo"] = selected_chessmen->getPlayer();
			json["board"]["cmsl"]["cm0"]["move"] = selected_chessmen->getHasMoved();
		}
		else {
			json["board"]["cmsl"]["count"] = 0;
		}
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
		json_output << std::setw(4) << json;
		json_output.close();
	}
	else {
		return FALSE;
	}
	return TRUE;
}

void chessfield::initGame() {
	last_game_status = cg::next;
	current_player = cg::white;
	selected_chessmen = nullptr;
	chessmen_onfield.clear();
	chessmen_onside.clear();
	backwardmovetrace.clear();
	chessmen_onfield.reserve(32);
	backwardmovetrace.reserve(32);
	forwardmovetrace.reserve(16);

	//ACTUAL CHESSBOARD
	//placing white chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(
				std::unique_ptr<chessmen>(std::make_unique<pawn>(cg::white, i, chessmen::fieldsize_y_start + 1))
			);
		}
		//placing rooks
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::white, 0, chessmen::fieldsize_y_start)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::white, 7, chessmen::fieldsize_y_start)));
		//placing knights
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::white, 1, chessmen::fieldsize_y_start)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::white, 6, chessmen::fieldsize_y_start)));
		//placing bishops															 
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::white, 2, chessmen::fieldsize_y_start)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::white, 5, chessmen::fieldsize_y_start)));
		//placing king
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<king>(cg::white, 3, chessmen::fieldsize_y_start)));
		//placing queen
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(cg::white, 4, chessmen::fieldsize_y_start)));
	}
	//placing black chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(
				std::unique_ptr<chessmen>(std::make_unique<pawn>(cg::black, i, chessmen::fieldsize_y_end - 1))
			);
		}
		//placing rooks
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::black, 0, chessmen::fieldsize_y_end)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<rook>(cg::black, 7, chessmen::fieldsize_y_end)));
		//placing knights
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::black, 1, chessmen::fieldsize_y_end)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<knight>(cg::black, 6, chessmen::fieldsize_y_end)));
		//placing bishops															
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::black, 2, chessmen::fieldsize_y_end)));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<bishop>(cg::black, 5, chessmen::fieldsize_y_end)));
		//placing king
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<king>(cg::black, 3, chessmen::fieldsize_y_end)));
		//placing queen
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(std::make_unique<queen>(cg::black, 4, chessmen::fieldsize_y_end)));
	}
}
