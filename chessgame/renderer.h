#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"
#include "gameEngine/chessfield/chessfield.h"

class renderer {
protected:
	virtual int gameLoop() { return TRUE; };
	virtual ~renderer() = default;
	renderer() = default;
};

class consoleRenderer final : public renderer {
protected:
	void render(chessfield& game);
	chessfield::game_status processOutput(chessfield::full_game_status status);
	static unsigned int translateY(const unsigned int& org_y);
	static unsigned int translateX(const unsigned int& org_x);
	static chessmen::position strtopos(std::string input);
	static std::string postostr(chessmen::position pos);

public:
	virtual int gameLoop() override;
	virtual ~consoleRenderer() override = default;
	consoleRenderer() = default;
};

class sfmlRenderer final : public renderer {
private:
	//loading board texture
	sf::Texture chessboard_txt;
	sf::Sprite chessboard_spr;
	//loading chessmen textures
	sf::Texture chessmen_txt;
	//asigning sprites
	sf::Sprite chessmen_king_white_spr;
	sf::Sprite chessmen_knight_white_spr;
	sf::Sprite chessmen_bishop_white_spr;
	sf::Sprite chessmen_pawn_white_spr;
	sf::Sprite chessmen_rook_white_spr;
	sf::Sprite chessmen_queen_white_spr;
	sf::Sprite chessmen_king_black_spr;
	sf::Sprite chessmen_knight_black_spr;
	sf::Sprite chessmen_bishop_black_spr;
	sf::Sprite chessmen_pawn_black_spr;
	sf::Sprite chessmen_rook_black_spr;
	sf::Sprite chessmen_queen_black_spr;
	//loading UI assets
	sf::Texture ui_back_txt;
	sf::Sprite ui_back_spr;
	sf::Texture ui_forward_txt;
	sf::Sprite ui_forward_spr;
	sf::Texture ui_save_txt;
	sf::Texture ui_load_txt;
	sf::Sprite ui_save_spr;
	sf::Sprite ui_load_spr;
	sf::Texture ui_retry_txt;
	sf::Sprite ui_retry_spr;
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

	static bool processUIInput(unsigned int ui_element, chessfield* game);
	static chessfield::game_status processOutput(chessfield& game, chessfield::full_game_status status);
	void render(chessfield& game, sf::RenderWindow & window);

public:
	std::string assets_image;
	std::string assets_font;

	virtual int gameLoop() override;
	virtual ~sfmlRenderer() override = default;
	sfmlRenderer();
};