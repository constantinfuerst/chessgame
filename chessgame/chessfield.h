#pragma once
#include "pch.h"
#include "chessmen.h"

class chessfield {
private:
	std::vector <chessmen> chessmen_onfield;
	std::vector <chessmen> chessmen_onside;
	void movetoside(chessmen::position& position);
public:
	chessfield();
};

void chessfield::movetoside(chessmen::position& position) {
	for (int i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i].current_position[0] == position[0] && chessmen_onfield[i].current_position[1] == position[1]) {
			chessmen_onside.push_back(chessmen_onfield[i]);
			chessmen_onfield.erase(chessmen_onfield.begin() + i);
		}
	}
}

chessfield::chessfield() {
	//placing white chessmen
	{
		//placing pawns
		for (int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(pawn(chessmen::white, { i, 1 }));
		}
		//placing rooks
		chessmen_onfield.push_back(rook(chessmen::white, { 0, 0 }));
		chessmen_onfield.push_back(rook(chessmen::white, { 7, 0 }));
		//placing knights
		chessmen_onfield.push_back(knight(chessmen::white, { 1, 0 }));
		chessmen_onfield.push_back(knight(chessmen::white, { 6, 0 }));
		//placing bishops
		chessmen_onfield.push_back(bishop(chessmen::white, { 2, 0 }));
		chessmen_onfield.push_back(bishop(chessmen::white, { 5, 0 }));
		//placing king
		chessmen_onfield.push_back(king(chessmen::white, { 3, 0 }));
		//placing queen
		chessmen_onfield.push_back(queen(chessmen::white, { 4, 0 }));
	}
	//placing black chessmen
	{
		//placing pawns
		for (int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(pawn(chessmen::black, { i, 6 }));
		}
		//placing rooks
		chessmen_onfield.push_back(rook(chessmen::black, { 0, 7 }));
		chessmen_onfield.push_back(rook(chessmen::black, { 7, 7 }));
		//placing knights
		chessmen_onfield.push_back(knight(chessmen::black, { 1, 7 }));
		chessmen_onfield.push_back(knight(chessmen::black, { 6, 7 }));
		//placing bishops
		chessmen_onfield.push_back(bishop(chessmen::black, { 2, 7 }));
		chessmen_onfield.push_back(bishop(chessmen::black, { 5, 7 }));
		//placing king
		chessmen_onfield.push_back(king(chessmen::black, { 3, 7 }));
		//placing queen
		chessmen_onfield.push_back(queen(chessmen::black, { 4, 7 }));
	}
}