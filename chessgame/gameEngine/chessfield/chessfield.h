#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"

struct notfound:std::exception {
	notfound() = default;
};

class chessfield {
public:
	typedef std::vector <std::unique_ptr<chessmen>> chessboard;
	typedef std::vector <chessmen*> ref_chessboard;
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
	typedef std::tuple<chessmen::position, chessmen::position> posswaptpl;
	typedef std::tuple<chessmen::position, posswaptpl> castelingtpl;
	typedef std::vector<castelingtpl> castelingvec;
	static chessboard copyChessboard(chessboard* chessboard_pntr);
	static void movetoside(chessmen::position& position, chessboard* virtual_field, chessboard* virtual_side);
	static void movetoempty(chessmen::position& old_position, chessmen::position& new_position, chessboard* field);
	king_status check_check(chessmen::color& player, chessboard* chessmen);
	king_status king_situation(chessmen::color player);
	move_sucess moveCharacter(chessmen::position& selectedMove, theoretical theoretical = oncetheoretical);
	move_sucess moveCasteling(chessmen::position& selectedMove, theoretical theoretical = oncetheoretical);
	chessmen* findChessmen(chessmen::position& position);
	static chessmen* findChessmen(chessmen::position position, chessboard* chessboard);
	castelingvec casteling(chessmen::color& player);
	void createChessmen(chessboard* chessboard, chessmen::chessfigure type, unsigned int posx, unsigned int posy, chessmen::color colo, bool move);
public:
	~chessfield();
	//this is what the player or gameloop interacts with
	full_game_status clickfield(chessmen::position field, chessmen::color player);
	void initGame(chessmen::color cur_player);
	bool initSaveGame(const std::string& filename);
	bool createSaveGame(const std::string& filename, chessmen::color current_player);
	//this is what the renderer uses to display legal moves
	std::vector<chessmen::position> truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate = FALSE);
};