#include "pch.h"
#include "chessfield.h"

chessmen* chessfield::findChessmen(chessmen::position& position) {
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->current_position[0] == position[0] && chessmen_onfield[i]->current_position[1] == position[1]) {
			return chessmen_onfield[i];
		}
	}
	throw notfound();
}

chessmen* chessfield::findChessmen(chessmen::position position, chessboard* chessboard) {
	for (size_t i = 0; i < chessboard->size(); i++) {
		if (chessboard->at(i)->current_position[0] == position[0] && chessboard->at(i)->current_position[1] == position[1]) {
			return chessboard->at(i);
		}
	}
	throw notfound();
}

chessfield::chessboard chessfield::copyChessboard(chessboard* chessboard_pntr) {
	chessboard returnboard;
	for (size_t i = 0; i < chessboard_pntr->size(); i++) {
		returnboard.push_back(chessboard_pntr->at(i)->clone());
	}
	return returnboard;
}

void chessfield::initGame() {
	chessmen_onfield.clear();
	chessmen_onside.clear();
	selected_chessmen.clear();

	//ACTUAL CHESSBOARD
	//placing white chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(new pawn(chessmen::white, { i, chessmen::fieldsize_y_start + 1 }));
		}
		//placing rooks
		chessmen_onfield.push_back(new rook(chessmen::white, { 0, chessmen::fieldsize_y_start }));
		chessmen_onfield.push_back(new rook(chessmen::white, { 7, chessmen::fieldsize_y_start }));
		//placing knights
		chessmen_onfield.push_back(new knight(chessmen::white, { 1, chessmen::fieldsize_y_start }));
		chessmen_onfield.push_back(new knight(chessmen::white, { 6, chessmen::fieldsize_y_start }));
		//placing bishops
		chessmen_onfield.push_back(new bishop(chessmen::white, { 2, chessmen::fieldsize_y_start }));
		chessmen_onfield.push_back(new bishop(chessmen::white, { 5, chessmen::fieldsize_y_start }));
		//placing king
		chessmen_onfield.push_back(new king(chessmen::white, { 3, chessmen::fieldsize_y_start }));
		//placing queen
		chessmen_onfield.push_back(new queen(chessmen::white, { 4, chessmen::fieldsize_y_start }));
	}
	//placing black chessmen
	{
		//placing pawns
		for (unsigned int i = 0; i < 8; i++) {
			chessmen_onfield.push_back(new pawn(chessmen::black, { i, chessmen::fieldsize_y_end - 1 }));
		}
		//placing rooks
		chessmen_onfield.push_back(new rook(chessmen::black, { 0, chessmen::fieldsize_y_end }));
		chessmen_onfield.push_back(new rook(chessmen::black, { 7, chessmen::fieldsize_y_end }));
		//placing knights
		chessmen_onfield.push_back(new knight(chessmen::black, { 1, chessmen::fieldsize_y_end }));
		chessmen_onfield.push_back(new knight(chessmen::black, { 6, chessmen::fieldsize_y_end }));
		//placing bishops
		chessmen_onfield.push_back(new bishop(chessmen::black, { 2, chessmen::fieldsize_y_end }));
		chessmen_onfield.push_back(new bishop(chessmen::black, { 5, chessmen::fieldsize_y_end }));
		//placing king
		chessmen_onfield.push_back(new king(chessmen::black, { 3, chessmen::fieldsize_y_end }));
		//placing queen
		chessmen_onfield.push_back(new queen(chessmen::black, { 4, chessmen::fieldsize_y_end }));
	}
}
