#include "pch.h"
#include "chessfield.h"

chessfield::~chessfield() {
	delete selected_chessmen;
	chessmen_onfield.clear();
	chessmen_onside.clear();
}

chessmen* chessfield::findChessmen(chessmen::position& position) {
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->current_position[0] == position[0] && chessmen_onfield[i]->current_position[1] == position[1]) {
			return chessmen_onfield[i].get();
		}
	}
	throw notfound();
}

chessmen* chessfield::findChessmen(chessmen::position position, chessboard* chessboard) {
	for (size_t i = 0; i < chessboard->size(); i++) {
		if (chessboard->at(i)->current_position[0] == position[0] && chessboard->at(i)->current_position[1] == position[1]) {
			return chessboard->at(i).get();
		}
	}
	throw notfound();
}

chessfield::chessboard chessfield::copyChessboard(chessboard* chessboard_pntr) {
	chessboard returnboard;
	for (size_t i = 0; i < chessboard_pntr->size(); i++) {
		returnboard.push_back(std::unique_ptr<chessmen>(chessboard_pntr->at(i)->clone()));
	}
	return returnboard;
}

bool chessfield::initSaveGame(const std::string& filename) {
	selected_chessmen = nullptr;
	chessmen_onfield.clear();
	chessmen_onside.clear();
	nlohmann::json json;
	std::fstream json_input;
	json_input.open(filename);
	if (json_input.is_open()) {
		json_input >> json;
		current_player = json["player"].get<chessmen::color>();
		for (int i = 0; i < json["cmof"]["count"].get<int>(); i++) {
			createChessmen(&chessmen_onfield, json["cmof"]["cm" + std::to_string(i)]["type"].get<chessmen::chessfigure>(), json["cmof"]["cm" + std::to_string(i)]["posx"].get<unsigned int>(), json["cmof"]["cm" + std::to_string(i)]["posy"].get<unsigned int>(), json["cmof"]["cm" + std::to_string(i)]["colo"].get<chessmen::color>(), json["cmof"]["cm" + std::to_string(i)]["move"].get<bool>());
		}
		for (int i = 0; i < json["cmos"]["count"].get<int>(); i++) {
			createChessmen(&chessmen_onside, json["cmos"]["cm" + std::to_string(i)]["type"].get<chessmen::chessfigure>(), json["cmos"]["cm" + std::to_string(i)]["posx"].get<unsigned int>(), json["cmos"]["cm" + std::to_string(i)]["posy"].get<unsigned int>(), json["cmos"]["cm" + std::to_string(i)]["colo"].get<chessmen::color>(), json["cmos"]["cm" + std::to_string(i)]["move"].get<bool>());
		}
		if (json["cmsl"]["count"].get<int>() == 1) {
			chessmen::position pos = { json["cmsl"]["cm0"]["posx"].get<int>(), json["cmsl"]["cm0"]["posy"].get<int>() };
			selected_chessmen = findChessmen(pos);
		}
		json_input.close();
	}
	else {
		return FALSE;
	}
	return TRUE;
}

void chessfield::createChessmen(chessboard* chessboard, chessmen::chessfigure type, unsigned int posx, unsigned int posy, chessmen::color colo, bool move) {
	if (type == chessmen::pawn)
		chessboard->push_back(std::unique_ptr<chessmen>(new pawn(colo, { posx, posy }, move)));
	else if (type == chessmen::rook)
		chessboard->push_back(std::unique_ptr<chessmen>(new rook(colo, { posx, posy }, move)));
	else if (type == chessmen::knight)
		chessboard->push_back(std::unique_ptr<chessmen>(new knight(colo, { posx, posy }, move)));
	else if (type == chessmen::bishop)
		chessboard->push_back(std::unique_ptr<chessmen>(new bishop(colo, { posx, posy }, move)));
	else if (type == chessmen::king)
		chessboard->push_back(std::unique_ptr<chessmen>(new king(colo, { posx, posy }, move)));
	else if (type == chessmen::queen)
		chessboard->push_back(std::unique_ptr<chessmen>(new queen(colo, { posx, posy }, move)));
	else
		return;
}

bool chessfield::createSaveGame(const std::string& filename, chessmen::color current_player) {
	nlohmann::json json;
	std::ofstream json_output;
	json_output.open(filename);
	if (json_output.is_open()) {
		json["player"] = current_player;
		json["cmof"]["count"] = chessmen_onfield.size();
		for (size_t i = 0; i < chessmen_onfield.size(); i++) {
			json["cmof"]["cm" + std::to_string(i)]["type"] = chessmen_onfield[i]->figure();
			json["cmof"]["cm" + std::to_string(i)]["posx"] = chessmen_onfield[i]->current_position[0];
			json["cmof"]["cm" + std::to_string(i)]["posy"] = chessmen_onfield[i]->current_position[1];
			json["cmof"]["cm" + std::to_string(i)]["colo"] = chessmen_onfield[i]->player_color;
			json["cmof"]["cm" + std::to_string(i)]["move"] = chessmen_onfield[i]->hasMoved;
		}
		json["cmos"]["count"] = chessmen_onside.size();
		for (size_t i = 0; i < chessmen_onside.size(); i++) {
			json["cmos"]["cm" + std::to_string(i)]["type"] = chessmen_onside[i]->figure();
			json["cmos"]["cm" + std::to_string(i)]["posx"] = chessmen_onside[i]->current_position[0];
			json["cmos"]["cm" + std::to_string(i)]["posy"] = chessmen_onside[i]->current_position[1];
			json["cmos"]["cm" + std::to_string(i)]["colo"] = chessmen_onside[i]->player_color;
			json["cmos"]["cm" + std::to_string(i)]["move"] = chessmen_onside[i]->hasMoved;
		}
		if (selected_chessmen != nullptr) {
			json["cmsl"]["count"] = 1;
			json["cmsl"]["cm0"]["type"] = selected_chessmen->figure();
			json["cmsl"]["cm0"]["posx"] = selected_chessmen->current_position[0];
			json["cmsl"]["cm0"]["posy"] = selected_chessmen->current_position[1];
			json["cmsl"]["cm0"]["colo"] = selected_chessmen->player_color;
			json["cmsl"]["cm0"]["move"] = selected_chessmen->hasMoved;
		}
		else {
			json["cmsl"]["count"] = 0;
		}
		json_output << json;
		json_output.close();
	}
	else {
		return FALSE;
	}
	return TRUE;
}

void chessfield::initGame(chessmen::color cur_player) {
	current_player = cur_player;
	selected_chessmen = nullptr;
	chessmen_onfield.clear();
	chessmen_onside.clear();

	//ACTUAL CHESSBOARD
	//placing white chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(std::unique_ptr<chessmen>(new pawn(chessmen::white, { i, chessmen::fieldsize_y_start + 1 })));
		}
		//placing rooks
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new rook(chessmen::white, { 0, chessmen::fieldsize_y_start })));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new rook(chessmen::white, { 7, chessmen::fieldsize_y_start })));
		//placing knights
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new knight(chessmen::white, { 1, chessmen::fieldsize_y_start })));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new knight(chessmen::white, { 6, chessmen::fieldsize_y_start })));
		//placing bishops
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new bishop(chessmen::white, { 2, chessmen::fieldsize_y_start })));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new bishop(chessmen::white, { 5, chessmen::fieldsize_y_start })));
		//placing king
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new king(chessmen::white, { 3, chessmen::fieldsize_y_start })));
		//placing queen
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new queen(chessmen::white, { 4, chessmen::fieldsize_y_start })));
	}
	//placing black chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(std::unique_ptr<chessmen>(new pawn(chessmen::black, { i, chessmen::fieldsize_y_end - 1 })));
		}
		//placing rooks
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new rook(chessmen::black, { 0, chessmen::fieldsize_y_end })));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new rook(chessmen::black, { 7, chessmen::fieldsize_y_end })));
		//placing knights
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new knight(chessmen::black, { 1, chessmen::fieldsize_y_end })));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new knight(chessmen::black, { 6, chessmen::fieldsize_y_end })));
		//placing bishops
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new bishop(chessmen::black, { 2, chessmen::fieldsize_y_end })));
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new bishop(chessmen::black, { 5, chessmen::fieldsize_y_end })));
		//placing king
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new king(chessmen::black, { 3, chessmen::fieldsize_y_end })));
		//placing queen
		chessmen_onfield.push_back(std::unique_ptr<chessmen>(new queen(chessmen::black, { 4, chessmen::fieldsize_y_end })));
	}
}
