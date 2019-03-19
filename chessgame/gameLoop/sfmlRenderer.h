#pragma once

#include "pch.h"

#include "gameEngine/chessfield/chessfield.h"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "settings.h"

class sfmlRenderer final {
private:
	//scaling the fields
	int chessboard_width;
	int chessboard_height;
	int field_height;
	int field_width;
	unsigned int screenWidth;
	unsigned int screenHeight;
	//UI elements
	int ui_element_width;
	std::vector<sf::Sprite*> ui_elements;
	//efficiency
	mutable cg::uidisplay displayingUI;
	mutable bool redraw = TRUE;
	//game status evaluator
	mutable cg::game_status game_status;

	void closeChild() const;
	void ui_child_create(cg::ui_type type, std::string message = "") const;
	void ui_child_addmessage(cg::gui_window child_window, const std::string & message) const;
	void ui_child_addloadsavegame(cg::gui_window child_window, int index) const;
	void ui_child_addcreatesavegame(cg::gui_window child_window) const;
	void ui_child_addnewgame(cg::gui_window child_window, std::string message) const;
	void render();

	static std::vector<std::string> getFiles();
	void createSavegame(tgui::EditBox::Ptr filename) const;
	void loadSavegame(std::string filename) const;
	bool processUIInput(unsigned int ui_element);
	cg::game_status processOutput(cg::full_game_status status);

	chessfield* game;
	tgui::Gui* gui;
	sf::RenderWindow* window;

public:
	int gameLoop();
	~sfmlRenderer();
	sfmlRenderer();
};