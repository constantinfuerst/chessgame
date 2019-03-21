#pragma once

#include "pch.h"
#include "gameEngine/cg_defs.h"
#include "gameEngine/chessmen/chessmen.h"
#include "moveData/moveData.h"

struct notfound:std::exception {
	notfound() = default;
};

class chessfield {
public:
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
	typedef std::vector <std::unique_ptr<move>> moveregister;

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
	void createChessmen(chessboard* chessboard, cg::chessfigure figure, cg::position pos, cg::color color, bool has_moved, move* movedata = nullptr);
	cg::king_status check_check(cg::color& player, chessboard* chessmen);
	cg::king_status king_situation(cg::color player);
	cg::move_sucess moveCharacter(cg::position& selectedMove, move* movecounter, cg::theoretical theoretical = cg::oncetheoretical);
	cg::move_sucess moveCasteling(cg::position& selectedMove, move* movecounter, cg::theoretical theoretical = cg::oncetheoretical);
	chessmen* findChessmen(cg::position& position);
	castelingvec casteling(cg::color player);
	std::vector<chessmen*> findmultipleChessmen(cg::chessfigure, cg::color);
	
public:
	chessfield();
	~chessfield();
	//this is what the gameloop interacts with
	cg::full_game_status clickfield(cg::position field, cg::color player);
	void initGame();
	void quit();
	void stepback();
	void stepforward();
	bool initSaveGame(const std::string& filename);
	bool createSaveGame(const std::string& filename, const std::string& gamename);
	chessboard* getField();
	chessboard* getSide();
	//this is what the renderer uses to display legal moves
	std::vector<cg::position> truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate = FALSE);
};