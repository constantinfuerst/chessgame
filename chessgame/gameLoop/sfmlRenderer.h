#pragma once

#include "pch.h"

#include "gameEngine/chessfield/chessfield.h"
#include "gameEngine/cg_defs.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

class sfmlRenderAsstes final {
private:
	cg::spr_ptr_vec ui_elements;
	cg::idim board_dims = { 0, 0 };
	cg::idim true_board_dims = { 0,0 };
	std::vector<cg::chessmen_sprite> sprites;

public:
	static sfmlRenderAsstes* get();
	void loadAssets();
	sf::Sprite* chessboard = nullptr;
	const cg::idim& getBoarddims() const;
	const cg::idim& getTrueBoarddims() const;
	cg::spr_ptr_vec& getUiElems();
	sf::Sprite* getSprite(cg::color player, cg::chessfigure figure) const;
};

class sfmlRenderer final {
private:
	//scaling the fields
	cg::idim field_dims;
	cg::idim screen_dims;
	//UI elements
	int ui_element_width;
	//efficiency
	mutable cg::uidisplay displayingUI;
	mutable bool redraw = TRUE;
	mutable bool paused = FALSE;
	//game status evaluator
	mutable cg::game_status game_status;
	//mousebutton
	bool lmb_press = FALSE;

	static std::vector<std::string> getFiles();

	void createSavegame(tgui::EditBox::Ptr filename) const;
	void loadSavegame(std::string filename) const;

	void closeChild() const;
	void ui_child_create(cg::ui_type type, std::string message = "") const;
	void ui_child_addmessage(cg::gui_window child_window, const std::string & message) const;
	void ui_child_addloadsavegame(cg::gui_window child_window, int index) const;
	void ui_child_addcreatesavegame(cg::gui_window child_window) const;
	void ui_child_addnewgame(cg::gui_window child_window, std::string message) const;

	bool processUIInput(unsigned int ui_element) const;
	void processOutput(cg::full_game_status status) const;

	chessfield* game;
	tgui::Gui* gui;
	sf::RenderWindow* window;

	void render();
	void gameLogic();

public:
	int gameLoop();
	~sfmlRenderer();
	sfmlRenderer();
};