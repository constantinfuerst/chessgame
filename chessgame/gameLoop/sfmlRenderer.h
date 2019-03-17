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
	enum uidisplay {
		nodisplay = 0, display_noupdate = 1, display_update = 2
	};
	mutable uidisplay displayingUI;
	mutable bool redraw = TRUE;
	//game status evaluator
	chessfield::game_status game_status;

	void ui_newgame(std::string message);
	void ui_message(std::string message) const;
	void ui_savegame();
	void ui_loadgame(int index = 0);
	void render();

	static std::vector<std::string> getFiles();
	void createSavegame(tgui::EditBox::Ptr filename) const;
	void loadSavegame(std::string filename) const;
	bool processUIInput(unsigned int ui_element);
	chessfield::game_status processOutput(chessfield::full_game_status status);

	chessfield* game;
	tgui::Gui* gui;
	sf::RenderWindow* window;

public:
	int gameLoop();
	~sfmlRenderer();
	sfmlRenderer();
};