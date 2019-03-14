#include "pch.h"
#include "moveData.h"

void move::removeChessmen(chessfield_info& board, chessmen::position pos) {
	for (size_t i = 0; i < board[0]->size(); i++) {
		if (board[0]->at(i)->board_position.x == pos.x && board[0]->at(i)->board_position.y == pos.y) {
			board[0]->erase(board[0]->begin() + i);
			break;
		}
	}
}

void move::placeBack(chessfield_info& board, chessmen::position pos) {
	for (size_t i = 0; i < board[1]->size(); i++) {
		if (board[1]->at(i)->board_position.x == pos.x && board[1]->at(i)->board_position.y == pos.x) {
			const chessmen::chessfigure type = board[1]->at(i)->figure();
			const chessmen::color colo = board[1]->at(i)->player_color;
			const unsigned int posx = board[1]->at(i)->board_position.x;
			const unsigned int posy = board[1]->at(i)->board_position.y;
			if (type == chessmen::rook)
				board[0]->push_back(std::unique_ptr<chessmen>(new rook(colo, { posx, posy }, TRUE)));
			else if (type == chessmen::knight)
				board[0]->push_back(std::unique_ptr<chessmen>(new knight(colo, { posx, posy }, TRUE)));
			else if (type == chessmen::pawn)
				board[0]->push_back(std::unique_ptr<chessmen>(new pawn(colo, { posx, posy }, TRUE)));
			else if (type == chessmen::bishop)
				board[0]->push_back(std::unique_ptr<chessmen>(new bishop(colo, { posx, posy }, TRUE)));
			else if (type == chessmen::king)
				board[0]->push_back(std::unique_ptr<chessmen>(new king(colo, { posx, posy }, TRUE)));
			else
				board[0]->push_back(std::unique_ptr<chessmen>(new queen(colo, { posx, posy }, TRUE)));
			board[1]->erase(board[1]->begin() + i);
			break;
		}
	}
}

void move::moveBack(chessfield_info& board, chessmen::position oldpos, chessmen::position currentpos, bool hasmovedold) {
	for (size_t i = 0; i < board[0]->size(); i++) {
		if (board[0]->at(i)->board_position.x == currentpos.x && board[0]->at(i)->board_position.y == currentpos.y) {
			board[0]->at(i)->board_position = oldpos;
			board[0]->at(i)->hasMoved = hasmovedold;
		}
	}
}

void move::traceBack(chessfield_info& field, move* movetrace) {
	while (!movetrace->changes.empty()) {
		chessmenMoved* move = &movetrace->changes.back();
		if (move->move == newcm) {
			removeChessmen(field, move->newPosition);
			field[0]->push_back(std::unique_ptr<chessmen>(new pawn(move->player, move->newPosition, TRUE)));
			movetrace->changes.pop_back(); movetrace->changes.pop_back();
		}
		else if (move->move == toempty) {
			moveBack(field, move->oldPosition, move->newPosition, move->hasmovedold);
			movetrace->changes.pop_back();
		}
		else if (move->move == toside) {
			placeBack(field, move->oldPosition);
			movetrace->changes.pop_back();
		}
		else {
			continue;
		}
	}
}