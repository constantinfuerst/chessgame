#pragma once

#include "pch.h"

class chessmen {
public:
	static const int fieldsize_x_end = 7;
	static const int fieldsize_x_start = 0;
	static const int fieldsize_y_end = 7;
	static const int fieldsize_y_start = 0;
	typedef std::array<unsigned int, 2> position;
	typedef int x_pos;
	typedef int y_pos;
	typedef unsigned int chessmen_id;

	enum color {
		white = 0, black = 1
	};
	enum chessfigure {
		pawn = 0, rook = 1, knight = 2, bishop = 3, queen = 4, king = 5, none = 6
	};
	enum position_status {
		empty = 0, friendly = 1, enemy = 2
	};

	virtual chessfigure figure() {
		return none;
	};

	position current_position = { 0, 0 };
	color player_color;
	bool hasMoved = FALSE;

	virtual std::vector<position> possibleMoves(std::vector<chessmen*>* chessmen);
	virtual chessmen* clone() const = 0;

	static position_status positiocheck(std::vector<chessmen*>* chessmen, position pos, color player);
	static bool validpos(chessmen::position position);

protected:
	chessmen(color color_input, position position_input) {
		current_position[0] = position_input[0];
		current_position[1] = position_input[1];
		player_color = color_input;
	}
};

inline bool chessmen::validpos(chessmen::position position) {
	if (position[0] >= fieldsize_x_start && position[0] <= fieldsize_x_end && position[1] >= fieldsize_y_start && position[1] <= fieldsize_y_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

inline std::vector<chessmen::position> chessmen::possibleMoves(std::vector<chessmen*>* chessmen) {
	std::vector<position> emptyreturn;
	return emptyreturn;
}

inline chessmen::position_status chessmen::positiocheck(std::vector<chessmen*>* chessmen, position pos, color player) {
	for (size_t i = 0; i < chessmen->size(); i++) {
		if (chessmen->at(i)->current_position[0] == pos[0] && chessmen->at(i)->current_position[1] == pos[1]) {
			if (chessmen->at(i)->player_color == player) {
				return friendly;
			}
			else {
				return enemy;
			}
		}
	}
	return empty;
}

class pawn : public chessmen {
public:
	pawn(color color_input, position position_input) : chessmen(color_input, position_input){}

	chessmen* clone() const override {
		return new pawn(*this);
	}

	virtual chessfigure figure() override {
		return chessmen::pawn;
	}
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override {
		std::vector<position> returnpos;
		position hitleft;
		position hitright;
		position onestep;
		position twostep;

		if (player_color == black) {
			hitleft[0] = current_position[0] - 1;
			hitleft[1] = current_position[1] - 1;
			hitright[0] = current_position[0] + 1;
			hitright[1] = current_position[1] - 1;
			onestep[0] = current_position[0];
			onestep[1] = current_position[1] - 1;
			twostep[0] = current_position[0];
			twostep[1] = current_position[1] - 2;
		}
		else if (player_color == white) {
			hitleft[0] = current_position[0] - 1;
			hitleft[1] = current_position[1] + 1;
			hitright[0] = current_position[0] + 1;
			hitright[1] = current_position[1] + 1;
			onestep[0] = current_position[0];
			onestep[1] = current_position[1] + 1;
			twostep[0] = current_position[0];
			twostep[1] = current_position[1] + 2;
		}

		if (current_position[1] <= fieldsize_y_end && current_position[1] >= fieldsize_y_start) {
			if (hasMoved == FALSE) {
				if (positiocheck(chessmen, onestep, player_color) == empty && positiocheck(chessmen, twostep, player_color) == empty && validpos(twostep)) {
					returnpos.push_back(twostep);
				}
			}
			if (positiocheck(chessmen, onestep, player_color) == empty && validpos(onestep)) {
				returnpos.push_back(onestep);
			}
			if (positiocheck(chessmen, hitleft, player_color) == enemy && validpos(hitleft)) {
				returnpos.push_back(hitleft);
			}
			if (positiocheck(chessmen, hitright, player_color) == enemy && validpos(hitright)) {
				returnpos.push_back(hitright);
			}
		}

		return returnpos;
	}
};

class rook : public chessmen {
public:
	rook(color color_input, position position_input) : chessmen(color_input, position_input) {}

	chessmen* clone() const override {
		return new rook(*this);
	}

	virtual chessfigure figure() override {
		return chessmen::rook;
	}
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override {
		std::vector<position> returnpos;
		{
			{
				int x = current_position[0] + 1;
				while (x <= fieldsize_x_end) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x++;
				}
			}
			{
				int x = current_position[0] - 1;
				while (x >= fieldsize_x_start) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x--;
				}
			}
			{
				int y = current_position[1] + 1;
				while (y <= fieldsize_y_end) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					y++;
				}
			}
			{
				int y = current_position[1] - 1;
				while (y >= fieldsize_y_start) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					y--;
				}
			}
		}
		return returnpos;
	}
};

class knight : public chessmen {
public:
	knight(color color_input, position position_input) : chessmen(color_input, position_input) {}

	chessmen* clone() const override {
		return new knight(*this);
	}

	virtual chessfigure figure() override {
		return chessmen::knight;
	}
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override {
		std::vector<position> returnpos;
		if (current_position[0] + 2 <= fieldsize_x_end) {
			if (current_position[1] + 1 <= fieldsize_y_end) {
				const position pos = { current_position[0] + 2, current_position[1] + 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[1] - 1 >= fieldsize_y_end) {
				const position pos = { current_position[0] + 2, current_position[1] - 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
		}
		if (current_position[0] - 2 >= fieldsize_x_start) {
			if (current_position[1] + 1 <= fieldsize_y_end) {
				const position pos = { current_position[0] - 2, current_position[1] + 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[1] - 1 >= fieldsize_y_start) {
				const position pos = { current_position[0] - 2, current_position[1] - 1 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
		}
		if (current_position[1] + 2 <= fieldsize_y_end) {
			if (current_position[0] + 1 <= fieldsize_x_end) {
				const position pos = { current_position[0] + 1, current_position[1] + 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[0] - 1 >= fieldsize_x_start) {
				const position pos = { current_position[0] - 1, current_position[1] + 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
		}
		if (current_position[1] - 2 >= fieldsize_y_start) {
			if (current_position[0] + 1 <= fieldsize_x_end) {
				const position pos = { current_position[0] + 1, current_position[1] - 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
			if (current_position[0] - 1 >= fieldsize_x_start) {
				const position pos = { current_position[0] - 1, current_position[1] - 2 };
				if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
					returnpos.push_back(pos);
				}
			}
		}
		return returnpos;
	}
};

class bishop : public chessmen {
public:
	bishop(color color_input, position position_input) : chessmen(color_input, position_input) {}

	chessmen* clone() const override {
		return new bishop(*this);
	}

	virtual chessfigure figure() override {
		return chessmen::bishop;
	}
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override {
		std::vector<position> returnpos;
		{
			{
				int x = current_position[0] + 1; int y = current_position[1] + 1;
				while (x <= fieldsize_x_end && y <= fieldsize_y_end) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x++; y++;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] + 1;
				while (x >= fieldsize_x_start && y <= fieldsize_y_end) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x--; y++;
				}
			}
			{
				int x = current_position[0] + 1; int y = current_position[1] - 1;
				while (x <= fieldsize_x_end && y >= fieldsize_y_start) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x++; y--;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] - 1;
				while (x >= fieldsize_x_start && y >= fieldsize_y_start) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x--; y--;
				}
			}

		}
		return returnpos;
	}
};

class queen : public chessmen {
public:
	queen(color color_input, position position_input) : chessmen(color_input, position_input) {}

	chessmen* clone() const override {
		return new queen(*this);
	}

	virtual chessfigure figure() override {
		return chessmen::queen;
	}
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override {
		std::vector<position> returnpos;
		{
			{
				int x = current_position[0] + 1; int y = current_position[1] + 1;
				while (x <= fieldsize_x_end && y <= fieldsize_y_end) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x++; y++;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] + 1;
				while (x >= fieldsize_x_start && y <= fieldsize_y_end) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x--; y++;
				}
			}
			{
				int x = current_position[0] + 1; int y = current_position[1] - 1;
				while (x <= fieldsize_x_end && y >= fieldsize_y_start) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x++; y--;
				}
			}
			{
				int x = current_position[0] - 1; int y = current_position[1] - 1;
				while (x >= fieldsize_x_start && y >= fieldsize_y_start) {
					const position pos = { x, y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x--; y--;
				}
			}
			{
				int x = current_position[0] + 1;
				while (x <= fieldsize_x_end) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x++;
				}
			}
			{
				int x = current_position[0] - 1;
				while (x >= fieldsize_x_start) {
					const position pos = { x, current_position[1] };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					x--;
				}
			}
			{
				int y = current_position[1] + 1;
				while (y <= fieldsize_y_end) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					y++;
				}
			}
			{
				int y = current_position[1] - 1;
				while (y >= fieldsize_y_start) {
					const position pos = { current_position[0], y };
					if (positiocheck(chessmen, pos, player_color) == empty && validpos(pos)) {
						returnpos.push_back(pos);
					}
					else if (positiocheck(chessmen, pos, player_color) == enemy && validpos(pos)) {
						returnpos.push_back(pos);
						break;
					}
					else {
						break;
					}
					y--;
				}
			}
		}
		return returnpos;
	}
};

class king : public chessmen {
public:
	king(color color_input, position position_input) : chessmen(color_input, position_input) {}

	chessmen* clone() const override {
		return new king(*this);
	}

	virtual chessfigure figure() override {
		return chessmen::king;
	}
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override {
		std::vector<position> returnpos;
		{
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (current_position[0] + i <= fieldsize_x_end && current_position[1] + j <= fieldsize_y_end) {
						const position pos = { current_position[0] + i ,current_position[1] + j };
						if (positiocheck(chessmen, pos, player_color) != friendly && validpos(pos)) {
							returnpos.push_back(pos);
						}
					}
				}
			}
		}
		return returnpos;
	}
};