#pragma once
#include "pch.h"

class chessmen {
public:
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
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

	virtual std::vector<position> possibleMoves(chessboard* chessmen);
	virtual chessmen* clone() const = 0;
	virtual ~chessmen() = default;

	static position_status positiocheck(chessboard* chessmen, position pos, color player);
	static bool validpos(chessmen::position position);

protected:
	chessmen(color color_input, position position_input, bool moved = false);
};

class pawn : public chessmen {
public:
	virtual ~pawn() override = default;
	pawn(color color_input, position position_input, bool move = FALSE) : chessmen(color_input, position_input, move){}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(chessboard* chessmen) override;
};

class rook : public chessmen {
public:
	virtual ~rook() override = default;
	rook(color color_input, position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(chessboard* chessmen) override;
};

class knight : public chessmen {
public:
	virtual ~knight() override = default;
	knight(color color_input, position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(chessboard* chessmen) override;
};

class bishop : public chessmen {
public:
	virtual ~bishop() override = default;
	bishop(color color_input, position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(chessboard* chessmen) override;
};

class queen : public chessmen {
public:
	virtual ~queen() override = default;
	queen(color color_input, position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(chessboard* chessmen) override;
};

class king : public chessmen {
public:
	virtual ~king() override = default;
	king(color color_input, position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	chessmen* clone() const override;
	virtual chessfigure figure() override;
	virtual std::vector<position> possibleMoves(chessboard* chessmen) override;
};