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
	chessmen(color color_input, position position_input);
};

class pawn : public chessmen {
public:
	pawn(color color_input, position position_input) : chessmen(color_input, position_input){}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override;
};

class rook : public chessmen {
public:
	rook(color color_input, position position_input) : chessmen(color_input, position_input) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override;
};

class knight : public chessmen {
public:
	knight(color color_input, position position_input) : chessmen(color_input, position_input) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override;
};

class bishop : public chessmen {
public:
	bishop(color color_input, position position_input) : chessmen(color_input, position_input) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override;
};

class queen : public chessmen {
public:
	queen(color color_input, position position_input) : chessmen(color_input, position_input) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override;
};

class king : public chessmen {
public:
	king(color color_input, position position_input) : chessmen(color_input, position_input) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(std::vector <chessmen*>* chessmen) override;
};