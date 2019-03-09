#include "pch.h"
#include "moveData.h"

void move::removeChessmen(chessfield_info& board, chessmen::position pos) {
	for (size_t i = 0; i < board[0]->size(); i++) {
		if (board[0]->at(i)->current_position[0] == pos[0] && board[0]->at(i)->current_position[1] == pos[1]) {
			board[0]->erase(board[0]->begin() + i);
			break;
		}
	}
}

void move::placeBack(chessfield_info& board, chessmen::position pos) {
	for (size_t i = 0; i < board[1]->size(); i++) {
		if (board[1]->at(i)->current_position[0] == pos[0] && board[1]->at(i)->current_position[1] == pos[1]) {
			const chessmen::chessfigure type = board[1]->at(i)->figure();
			const chessmen::color colo = board[1]->at(i)->player_color;
			const unsigned int posx = board[1]->at(i)->current_position[0];
			const unsigned int posy = board[1]->at(i)->current_position[1];
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

void move::moveBack(chessfield_info& board, chessmen::position oldpos, chessmen::position currentpos) {
	for (size_t i = 0; i < board[0]->size(); i++) {
		if (board[0]->at(i)->current_position[0] == currentpos[0] && board[0]->at(i)->current_position[1] == currentpos[1]) {
			board[0]->at(i)->current_position = oldpos;
		}
	}
}

void move::traceBack(chessfield_info& board, move* movetrace) {
	while (!movetrace->changes.empty()) {
		chessmenMoved* move = &movetrace->changes.back();
		if (move->move == newcm) {
			removeChessmen(board, move->newPosition);
			board[0]->push_back(std::unique_ptr<chessmen>(new pawn(move->player, move->newPosition, TRUE)));
			movetrace->changes.pop_back(); movetrace->changes.pop_back();
		}
		else if (move->move == toempty) {
			moveBack(board, move->oldPosition, move->newPosition);
			movetrace->changes.pop_back();
		}
		else if (move->move == toside) {
			placeBack(board, move->oldPosition);
			movetrace->changes.pop_back();
		}
		else {
			continue;
		}
	}
}