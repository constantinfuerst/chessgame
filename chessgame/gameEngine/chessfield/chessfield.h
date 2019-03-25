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
#include "gameEngine/chessmen/chessmen.h"
#include "moveData/moveData.h"

struct notfound final : std::exception {
	notfound() = default;
};

class chessfield {
public:
	typedef std::vector<std::unique_ptr<chessmen>> chessboard;
	typedef std::vector<std::unique_ptr<move>> moveregister;

	chessmen* selected_chessmen = nullptr;
	cg::color current_player;

protected:
	chessboard chessmen_onfield;
	chessboard chessmen_onside;

private:
	struct casteling_str {
		cg::position oldrpos;
		cg::position newrpos;
		cg::position newkpos;
	};

	typedef std::vector<casteling_str> castelingvec;
	cg::full_game_status last_game_status;

	moveregister backwardmovetrace;
	moveregister forwardmovetrace;

	static chessboard copyChessboard(chessboard* chessboard_pntr);
	static void movetoside(cg::position& position, chessboard* virtual_field, chessboard* virtual_side, move* movedata);
	static void movetoempty(cg::position& old_position, cg::position& new_position, chessboard* field, move* movedata);
	static chessmen* findChessmen(cg::position position, chessboard* chessboard);
	void createChessmen(chessboard* chessboard, cg::chessfigure figure, cg::position pos, cg::color color,
	                    bool has_moved, move* movedata = nullptr);
	cg::king_status check_check(cg::color& player, chessboard* chessmen);
	cg::king_status king_situation(cg::color player);
	cg::move_sucess moveCharacter(cg::position& selectedMove, move* movecounter,
	                              cg::theoretical theoretical = cg::oncetheoretical);
	cg::move_sucess moveCasteling(cg::position& selectedMove, move* movecounter,
	                              cg::theoretical theoretical = cg::oncetheoretical);
	chessmen* findChessmen(cg::position& position);
	castelingvec casteling(cg::color player);
	std::vector<chessmen*> findmultipleChessmen(cg::chessfigure, cg::color);

public:
	chessfield();
	~chessfield();
	//this is what the game loop interacts with
	cg::full_game_status clickfield(cg::position field, cg::color player);
	void initGame();
	void quit();
	void clean();
	void stepback();
	void stepforward();
	bool initSaveGame(const std::string& filename);
	bool createSaveGame(const std::string& filename, const std::string& gamename);
	chessboard* getField();
	chessboard* getSide();
	//this is what the renderer uses to display legal moves
	std::vector<cg::position> truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate = FALSE);
};
