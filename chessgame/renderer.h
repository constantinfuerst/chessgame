#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"
#include "gameEngine/chessfield/chessfield.h"
#include <SFML/Graphics.hpp>
#include "TGUI/TGUI.hpp"

class renderer {
protected:
	virtual int gameLoop() = 0;
	virtual ~renderer() = default;
	renderer() = default;
};

class consoleRenderer final : public renderer {
protected:
	static chessfield::game_status processOutput(chessfield::full_game_status status);
	static unsigned int translateY(const unsigned int& org_y);
	static unsigned int translateX(const unsigned int& org_x);
	static chessmen::position strtopos(std::string input);
	static std::string postostr(chessmen::position pos);
	static void render(chessfield& game);

public:
	virtual int gameLoop() override;
	virtual ~consoleRenderer() override = default;
	consoleRenderer() = default;
};

class sfmlRenderer final : public renderer {
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
	//efficiency bools
	bool displayingUI;
	bool redraw = TRUE;
	//game status evaluator
	chessfield::game_status game_status;

	void ui_newgame(tgui::Gui* gui, std::string message);
	void render(chessfield* game, sf::RenderWindow* window);

	bool createSavegame(chessfield* game, tgui::Gui* gui) const;
	bool loadSavegame(chessfield* game, tgui::Gui* gui) const;
	bool processUIInput(unsigned int ui_element, chessfield* game, tgui::Gui* gui);
	chessfield::game_status processOutput(chessfield& game, chessfield::full_game_status status, tgui::Gui* gui);

public:
	std::string assets_image;
	std::string assets_font;

	virtual int gameLoop() override;
	virtual ~sfmlRenderer() override = default;
	sfmlRenderer();
};