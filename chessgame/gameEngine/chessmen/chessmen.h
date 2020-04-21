#pragma once
#include "pch.h"

#include "gameEngine/cg_defs.h"

class chessmen {
protected:
	cg::position board_position = {0, 0};
	cg::color player_color = cg::white;
	bool hasMoved = FALSE;

public:
	typedef std::vector<std::unique_ptr<chessmen>> chessboard;

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
	void setPos(cg::position pos);
	cg::position& setPos();
};

class pawn : public chessmen {
public:
	~pawn() override = default;

	pawn(const cg::color color_input, const cg::position position_input, const bool move = FALSE) : chessmen(
		color_input, position_input, move) {
	}

	pawn(const cg::color color_input, const unsigned int posx, const unsigned int posy, const bool move = FALSE) : chessmen(
		color_input, posx, posy, move) {
	}

	chessmen* clone() const override;
	cg::chessfigure figure() override;
	std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class rook : public chessmen {
public:
	~rook() override = default;

	rook(const cg::color color_input, const cg::position position_input, const bool move = FALSE) : chessmen(
		color_input, position_input, move) {
	}

	rook(const cg::color color_input, const unsigned int posx, const unsigned int posy, const bool move = FALSE) : chessmen(
		color_input, posx, posy, move) {
	}

	chessmen* clone() const override;
	cg::chessfigure figure() override;
	std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class knight : public chessmen {
public:
	~knight() override = default;

	knight(const cg::color color_input, const cg::position position_input, const bool move = FALSE) : chessmen(
		color_input, position_input, move) {
	}

	knight(const cg::color color_input, const unsigned int posx, const unsigned int posy, const bool move = FALSE) : chessmen(
		color_input, posx, posy, move) {
	}

	chessmen* clone() const override;
	cg::chessfigure figure() override;
	std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class bishop : public chessmen {
public:
	~bishop() override = default;

	bishop(const cg::color color_input, const cg::position position_input, const bool move = FALSE) : chessmen(
		color_input, position_input, move) {
	}

	bishop(const cg::color color_input, const unsigned int posx, const unsigned int posy, const bool move = FALSE) : chessmen(
		color_input, posx, posy, move) {
	}

	chessmen* clone() const override;
	cg::chessfigure figure() override;
	std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class queen : public chessmen {
public:
	~queen() override = default;

	queen(const cg::color color_input, const cg::position position_input, const bool move = FALSE) : chessmen(
		color_input, position_input, move) {
	}

	queen(const cg::color color_input, const unsigned int posx, const unsigned int posy, const bool move = FALSE) : chessmen(
		color_input, posx, posy, move) {
	}

	chessmen* clone() const override;
	cg::chessfigure figure() override;
	std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};

class king : public chessmen {
public:
	~king() override = default;

	king(const cg::color color_input, const cg::position position_input, const bool move = FALSE) : chessmen(
		color_input, position_input, move) {
	}

	king(const cg::color color_input, const unsigned int posx, const unsigned int posy, const bool move = FALSE) : chessmen(
		color_input, posx, posy, move) {
	}

	chessmen* clone() const override;
	cg::chessfigure figure() override;
	std::vector<cg::position> possibleMoves(chessboard* chessmen) override;
};
