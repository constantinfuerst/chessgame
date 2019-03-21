/*
PROGRAMM METADATA:
	name:           chessgame
	description:    chessgame project I made to learn OOP, using sfml combined with tgui or a console as output
	copyright:      (c) 2019 Constantin Fürst constantin@fuersten.info
LICENSE:
   This work is created by Constantin Fürst.
   If you are unsure if your use of this project is permitted or you'd like to use it in a way prohibited by the following license, please message "constantin@fuersten.info".
   This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
   To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-nd/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
DISCLAIMER:
THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS 'AS IS' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
 */

#pragma once
#include "pch.h"
#include "gameEngine/cg_defs.h"

class chessmen {
protected:
	cg::position board_position = { 0, 0 };
	cg::color player_color;
	bool hasMoved = FALSE;

public:
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;

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