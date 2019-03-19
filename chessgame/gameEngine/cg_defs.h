#pragma once

#include "pch.h"
#include <TGUI/TGUI.hpp>
#include "gameLoop/settings.h"

namespace cg {
	//TYPEDEFS
	typedef std::vector<sf::Sprite*> spr_ptr_vec;

	//ENUMS
	enum full_game_status {
		error, enemy, selected, next, ownchessmen, emptyfield, checked, impmove, bkstale, wkstale, bkmate, wkmate
	};
	enum game_status {
		end, restart, running, mistake
	};
	enum king_status {
		not_check = 0, check = 1, stale = 2, checkmate = 3
	};
	enum move_sucess {
		impossible = 0, wouldbecheck = 1, sucess = 2
	};
	enum theoretical {
		nontheoretical = 0, oncetheoretical = 1, onlytheoretical = 2
	};
	enum uidisplay {
		nodisplay = 0, display_noupdate = 1, display_update = 2
	};
	enum moveType {
		toempty = 0, toside = 1, newcm = 2
	};
	enum color {
		white = 0, black = 1
	};
	enum chessfigure {
		pawn = 0, rook = 1, knight = 2, bishop = 3, queen = 4, king = 5, none = 6
	};
	enum position_status {
		empty_pos = 0, friendly_pos = 1, enemy_pos = 2
	};
	enum ui_type {
		ui_message = 0, ui_savegame = 1, ui_loadgame = 2, ui_newgame = 3
	};

	//STRUCTS
	struct position {
		unsigned int x;
		unsigned int y;
	};
	struct idim {
		unsigned int width;
		unsigned int height;
	};
	struct fdim {
		float width;
		float height;
	};
	struct gui_window {
		tgui::Theme theme;
		std::shared_ptr<tgui::ChildWindow> child_window;
		unsigned int width;
		unsigned int height;
		unsigned int padding;
	};
	struct chessmen_sprite {
		color player;
		chessfigure figure;
		sf::Sprite* sprite;
	};
}