#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"
#include "moveData/moveData.h"

struct notfound:std::exception {
	notfound() = default;
};

class chessfield {
public:
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
	typedef std::vector <std::unique_ptr<move>> moveregister;

	enum full_game_status {
		error, enemy, selected, next, ownchessmen, emptyfield, checked, impmove, bkstale, wkstale, bkmate, wkmate
	};
	enum game_status {
		end, running, mistake
	};

	chessboard chessmen_onfield;
	chessboard chessmen_onside;
	chessmen* selected_chessmen = nullptr;
	chessmen::color current_player;

private:
	enum king_status {
		none = 0, check = 1, stale = 2, checkmate = 3
	};
	enum move_sucess {
		impossible = 0, wouldbecheck = 1, sucess = 2
	};
	enum theoretical {
		nontheoretical = 0, oncetheoretical = 1, onlytheoretical = 2
	};

	struct casteling_str {
		chessmen::position oldrpos;
		chessmen::position newrpos;
		chessmen::position newkpos;
	};

	typedef std::vector<casteling_str> castelingvec;

	moveregister backwardmovetrace;
	moveregister forwardmovetrace;

	static chessboard copyChessboard(chessboard* chessboard_pntr);
	static void createChessmen(chessboard* chessboard, chessmen::chessfigure type, chessmen::position pos, chessmen::color colo, bool move);
	static void movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side, move* movedata, bool register_move);
	static void movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field, move* movedata, bool register_move);
	void newchessmen(chessmen::position& position, move* movedata, chessmen::color color, chessmen::chessfigure figure, bool register_move);
	king_status check_check(chessmen::color& player, chessboard* chessmen);
	king_status king_situation(chessmen::color player);
	move_sucess moveCharacter(chessmen::position& selectedMove, move* movecounter, theoretical theoretical = oncetheoretical);
	move_sucess moveCasteling(chessmen::position& selectedMove, move* movecounter, theoretical theoretical = oncetheoretical);
	chessmen* findChessmen(chessmen::position& position);
	static chessmen* findChessmen(chessmen::position position, chessboard* chessboard);
	castelingvec casteling(chessmen::color& player);
	
public:
	chessfield();
	~chessfield();
	//this is what the gameloop interacts with
	full_game_status clickfield(chessmen::position field, chessmen::color player);
	void initGame();
	void quit();
	void stepback();
	void stepforward();
	bool initSaveGame(const std::string& filename);
	bool createSaveGame(const std::string& filename);
	//this is what the renderer uses to display legal moves
	std::vector<chessmen::position> truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate = FALSE);
};