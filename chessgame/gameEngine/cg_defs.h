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
#include <TGUI/TGUI.hpp>
#include "gameLoop/settings.h"

namespace cg {
	//"GLOBALS"
	const int fieldsize_x_end = FIELDSIZE;
	const int fieldsize_x_start = 0;
	const int fieldsize_y_end = FIELDSIZE;
	const int fieldsize_y_start = 0;

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