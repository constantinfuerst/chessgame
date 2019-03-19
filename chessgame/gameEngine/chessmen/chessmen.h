#pragma once
#include "pch.h"
#include "gameEngine/cg_defs.h"

class chessmen {
public:
	//PUBLIC DEFINITIONS
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
	static const int fieldsize_x_end = 7;
	static const int fieldsize_x_start = 0;
	static const int fieldsize_y_end = 7;
	static const int fieldsize_y_start = 0;

protected:
	cg::position board_position = { 0, 0 };
	cg::color player_color;
	bool hasMoved = FALSE;

public:
	//truly public methods
	virtual chessmen* clone() const = 0;
	virtual cg::chessfigure figure() = 0;
	virtual ~chessmen() = default;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) = 0;

	static cg::position_status positiocheck(chessboard* chessmen, cg::position pos, cg::color player);
	static bool validpos(cg::position position);

	chessmen(cg::color color_input, cg::position position_input, bool moved = false);
	chessmen(cg::color color_input, unsigned int posx, unsigned int posy, bool moved = false);

	cg::position getPos() const;
	cg::color getPlayer() const;
	bool getHasMoved() const;

	void setHasMoved(bool hasMoved);
	void setPos(const cg::position pos);
	cg::position& setPos();
};

class pawn : public chessmen {
public:
	virtual ~pawn() override = default;
	pawn(cg::color color_input, cg::position position_input, bool move = FALSE) : chessmen(color_input, position_input, move){}
	pawn(cg::color color_input, unsigned int posx, unsigned int posy, bool move = FALSE) : chessmen(color_input, posx, posy, move) {}
	chessmen* clone() const override;
	cg::chessfigure figure() override;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class rook : public chessmen {
public:
	virtual ~rook() override = default;
	rook(cg::color color_input, cg::position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	rook(cg::color color_input, unsigned int posx, unsigned int posy, bool move = FALSE) : chessmen(color_input, posx, posy, move) {}
	chessmen* clone() const override;
	virtual cg::chessfigure figure() override;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class knight : public chessmen {
public:
	virtual ~knight() override = default;
	knight(cg::color color_input, cg::position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	knight(cg::color color_input, unsigned int posx, unsigned int posy, bool move = FALSE) : chessmen(color_input, posx, posy, move) {}
	chessmen* clone() const override;
	virtual cg::chessfigure figure() override;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class bishop : public chessmen {
public:
	virtual ~bishop() override = default;
	bishop(cg::color color_input, cg::position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	bishop(cg::color color_input, unsigned int posx, unsigned int posy, bool move = FALSE) : chessmen(color_input, posx, posy, move) {}
	chessmen* clone() const override;
	virtual cg::chessfigure figure() override;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class queen : public chessmen {
public:
	virtual ~queen() override = default;
	queen(cg::color color_input, cg::position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	queen(cg::color color_input, unsigned int posx, unsigned int posy, bool move = FALSE) : chessmen(color_input, posx, posy, move) {}
	chessmen* clone() const override;
	virtual cg::chessfigure figure() override;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class king : public chessmen {
public:
	virtual ~king() override = default;
	king(cg::color color_input, cg::position position_input, bool move = FALSE) : chessmen(color_input, position_input, move) {}
	king(cg::color color_input, unsigned int posx, unsigned int posy, bool move = FALSE) : chessmen(color_input, posx, posy, move) {}
	chessmen* clone() const override;
	virtual cg::chessfigure figure() override;
	virtual std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};