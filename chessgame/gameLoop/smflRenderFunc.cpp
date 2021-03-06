#include "pch.h"

#include "sfmlRenderer.h"
#include "settings.h"

#ifdef WIN_DESKTOP_GUI
	#define cout(str)
#else
#include <iostream>
#define cout(str) std::cout << str << std::endl;
#endif

//returns singleton instance
sfmlRenderAsstes* sfmlRenderAsstes::get() {
	static sfmlRenderAsstes instance;
	return &instance;
}

//returns dimensions of the loaded chessboard texture
const cg::idim& sfmlRenderAsstes::getBoarddims() const {
	return board_dims;
}

//returns dimensions of the loaded chessboard texture
const cg::idim& sfmlRenderAsstes::getTrueBoarddims() const {
	return true_board_dims;
}

//returns pointer to the UI elements sprite
cg::spr_ptr_vec& sfmlRenderAsstes::getUiElems() {
	return ui_elements;
}

//returns a sprite with matching color and piece
sf::Sprite* sfmlRenderAsstes::getSprite(const cg::color player, const cg::chessfigure figure) const {
	for (auto sprite : sprites) {
		if (sprite.figure == figure && sprite.player == player) {
			return sprite.sprite;
		}
	}
	return nullptr;
}

void sfmlRenderer::render() {
	//if the game hasn't been instantiated we cant render it
	if (game == nullptr)
		return;

	//////////////////
	// DRAW ROUTINE //
	//////////////////

	//grab the UI elements
	auto ui_elements = sfmlRenderAsstes::get()->getUiElems();

	//draw UI
	//add a kingtexture from the current player to show whose turn it is
	ui_elements.push_back(new sf::Sprite(*sfmlRenderAsstes::get()->getSprite(game->current_player, cg::king)));

	int ui_height = screen_dims.height - sfmlRenderAsstes::get()->getTrueBoarddims().height;
	int ui_width = screen_dims.width;
	ui_element_width = ui_width / ui_elements.size() - 16;

	for (unsigned int i = 0; i < ui_elements.size(); i++) {
		cg::fdim size = {
			static_cast<float>(ui_elements[i]->getTextureRect().width),
			static_cast<float>(ui_elements[i]->getTextureRect().height)
		};
		cg::fdim scale = {
			static_cast<float>(ui_element_width) / static_cast<float>(size.width),
			static_cast<float>(ui_height) / static_cast<float>(size.height)
		};
		cg::position pos = {
			i * ui_element_width + 32,
			screen_dims.height - ui_height
		};
		ui_elements[i]->setColor(sf::Color::White);
		ui_elements[i]->setScale({scale.width, scale.height});
		ui_elements[i]->setPosition({
			static_cast<float>(pos.x),
			static_cast<float>(pos.y)
		});
		window->draw(*ui_elements[i]);
	}

	delete ui_elements.back();
	ui_elements.pop_back();

	//draw game

	window->draw(*sfmlRenderAsstes::get()->chessboard);

	//if a chessmen is selected
	if (game->selected_chessmen != nullptr) {
		//draw the field of the selected chessmen in lime green
		sf::RectangleShape selected(sf::Vector2f(field_dims.width, field_dims.height));
		selected.setFillColor(sf::Color(25, 225, 0, 225));
		selected.setPosition(
			static_cast<float>(game->selected_chessmen->getPos().x * field_dims.width + 28),
			static_cast<float>(game->selected_chessmen->getPos().y * field_dims.height + 28)
		);
		window->draw(selected);
		//draw every possible move of the selected chessmen in a darker green
		auto possibleMoves = game->truePossibleMoves(game->selected_chessmen, game->getField());
		for (auto& possibleMove : possibleMoves) {
			sf::RectangleShape pmindicator(sf::Vector2f(field_dims.width, field_dims.height));
			pmindicator.setFillColor(sf::Color(50, 100, 0, 225));
			pmindicator.setPosition(
				static_cast<float>(possibleMove.x * field_dims.width + 28),
				static_cast<float>(possibleMove.y * field_dims.height + 28)
			);
			window->draw(pmindicator);
		}
	}

	for (size_t i = 0; i < game->getField()->size(); i++) {

	}

	//for every chessmen on the field
	for (auto& i : *game->getField()) {
		sf::Sprite* current_sprite = sfmlRenderAsstes::get()->getSprite(i->getPlayer(), i->figure());
		//if the sprite we got exists draw it at the position of the chessmen we are drawing
		if (current_sprite != nullptr) {
			current_sprite->setPosition(
				static_cast<float>(i->getPos().x * field_dims.width + 28),
				static_cast<float>(i->getPos().y * field_dims.height + 28)
			);
			window->draw(*current_sprite);
		}
		else {
			cout("nullptr draw");
		}
	}
}

void sfmlRenderAsstes::loadAssets() {
	//loading board texture
	static sf::Texture chessboard_txt;
	static sf::Sprite chessboard_spr;
	//loading chessmen textures
	static sf::Texture chessmen_txt;
	//assigning sprites
	static sf::Sprite chessmen_king_white_spr;
	static sf::Sprite chessmen_knight_white_spr;
	static sf::Sprite chessmen_bishop_white_spr;
	static sf::Sprite chessmen_pawn_white_spr;
	static sf::Sprite chessmen_rook_white_spr;
	static sf::Sprite chessmen_queen_white_spr;
	static sf::Sprite chessmen_king_black_spr;
	static sf::Sprite chessmen_knight_black_spr;
	static sf::Sprite chessmen_bishop_black_spr;
	static sf::Sprite chessmen_pawn_black_spr;
	static sf::Sprite chessmen_rook_black_spr;
	static sf::Sprite chessmen_queen_black_spr;
	//loading UI assets
	static sf::Texture ui_back_txt;
	static sf::Sprite ui_back_spr;
	static sf::Texture ui_forward_txt;
	static sf::Sprite ui_forward_spr;
	static sf::Texture ui_save_txt;
	static sf::Texture ui_load_txt;
	static sf::Sprite ui_save_spr;
	static sf::Sprite ui_load_spr;
	static sf::Texture ui_retry_txt;
	static sf::Sprite ui_retry_spr;

	cout("constructing renderer assets");

	//loading board texture
	chessboard_txt.loadFromFile(ASSETS_DIR + "images\\board.png");
	chessboard_spr.setTexture(chessboard_txt);
	chessboard = &chessboard_spr;

	//loading chessmen textures
	chessmen_txt.loadFromFile(ASSETS_DIR + "images\\figures.png");
	//assigning sprites
	chessmen_king_white_spr.setTexture(chessmen_txt);
	chessmen_king_white_spr.setTextureRect(sf::IntRect(224, 56, 56, 56));
	sprites.push_back({cg::white, cg::king, &chessmen_king_white_spr});
	chessmen_knight_white_spr.setTexture(chessmen_txt);
	chessmen_knight_white_spr.setTextureRect(sf::IntRect(56, 56, 56, 56));
	sprites.push_back({cg::white, cg::knight, &chessmen_knight_white_spr});
	chessmen_bishop_white_spr.setTexture(chessmen_txt);
	chessmen_bishop_white_spr.setTextureRect(sf::IntRect(112, 56, 56, 56));
	sprites.push_back({cg::white, cg::bishop, &chessmen_bishop_white_spr});
	chessmen_pawn_white_spr.setTexture(chessmen_txt);
	chessmen_pawn_white_spr.setTextureRect(sf::IntRect(280, 56, 56, 56));
	sprites.push_back({cg::white, cg::pawn, &chessmen_pawn_white_spr});
	chessmen_rook_white_spr.setTexture(chessmen_txt);
	chessmen_rook_white_spr.setTextureRect(sf::IntRect(0, 56, 56, 56));
	sprites.push_back({cg::white, cg::rook, &chessmen_rook_white_spr});
	chessmen_queen_white_spr.setTexture(chessmen_txt);
	chessmen_queen_white_spr.setTextureRect(sf::IntRect(168, 56, 56, 56));
	sprites.push_back({cg::white, cg::queen, &chessmen_queen_white_spr});
	chessmen_king_black_spr.setTexture(chessmen_txt);
	chessmen_king_black_spr.setTextureRect(sf::IntRect(224, 0, 56, 56));
	sprites.push_back({cg::black, cg::king, &chessmen_king_black_spr});
	chessmen_knight_black_spr.setTexture(chessmen_txt);
	chessmen_knight_black_spr.setTextureRect(sf::IntRect(56, 0, 56, 56));
	sprites.push_back({cg::black, cg::knight, &chessmen_knight_black_spr});
	chessmen_bishop_black_spr.setTexture(chessmen_txt);
	chessmen_bishop_black_spr.setTextureRect(sf::IntRect(112, 0, 56, 56));
	sprites.push_back({cg::black, cg::bishop, &chessmen_bishop_black_spr});
	chessmen_pawn_black_spr.setTexture(chessmen_txt);
	chessmen_pawn_black_spr.setTextureRect(sf::IntRect(280, 0, 56, 56));
	sprites.push_back({cg::black, cg::pawn, &chessmen_pawn_black_spr});
	chessmen_rook_black_spr.setTexture(chessmen_txt);
	chessmen_rook_black_spr.setTextureRect(sf::IntRect(0, 0, 56, 56));
	sprites.push_back({cg::black, cg::rook, &chessmen_rook_black_spr});
	chessmen_queen_black_spr.setTexture(chessmen_txt);
	chessmen_queen_black_spr.setTextureRect(sf::IntRect(168, 0, 56, 56));
	sprites.push_back({cg::black, cg::queen, &chessmen_queen_black_spr});

	const sf::Vector2u chessboard_size = chessboard_txt.getSize();
	board_dims.width = chessboard_size.x - 28 * 2;
	board_dims.height = chessboard_size.y - 28 * 2;
	true_board_dims.width = chessboard_size.x;
	true_board_dims.height = chessboard_size.y;

	//loading UI texture
	ui_back_txt.loadFromFile(ASSETS_DIR + "images\\back.png");
	ui_back_spr.setTexture(ui_back_txt);
	ui_forward_txt.loadFromFile(ASSETS_DIR + "images\\forward.png");
	ui_forward_spr.setTexture(ui_forward_txt);
	ui_save_txt.loadFromFile(ASSETS_DIR + "images\\save.png");
	ui_save_spr.setTexture(ui_save_txt);
	ui_load_txt.loadFromFile(ASSETS_DIR + "images\\load.png");
	ui_load_spr.setTexture(ui_load_txt);
	ui_retry_txt.loadFromFile(ASSETS_DIR + "images\\retry.png");
	ui_retry_spr.setTexture(ui_retry_txt);

	ui_elements.reserve(6);
	ui_elements.push_back(&ui_back_spr);
	ui_elements.push_back(&ui_forward_spr);
	ui_elements.push_back(&ui_save_spr);
	ui_elements.push_back(&ui_load_spr);
	ui_elements.push_back(&ui_retry_spr);
}
