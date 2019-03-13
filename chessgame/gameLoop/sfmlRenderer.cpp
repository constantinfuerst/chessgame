#include "pch.h"
#include "renderer.h"

sfmlRenderer::sfmlRenderer() {
	assets_image = R"(C:\Users\Constantin\source\repos\chessgame\assets\images\)";
	assets_font= R"(C:\Users\Constantin\source\repos\chessgame\assets\fonts\)";

	//loading board texture
	chessboard_txt.loadFromFile(assets_image + "board.png");
	chessboard_spr.setTexture(chessboard_txt);
	//loading chessmen textures
	chessmen_txt.loadFromFile(assets_image + "figures.png");
	//asigning sprites
	chessmen_king_white_spr.setTexture(chessmen_txt);
	chessmen_king_white_spr.setTextureRect(sf::IntRect(224, 56, 56, 56));
	chessmen_knight_white_spr.setTexture(chessmen_txt);
	chessmen_knight_white_spr.setTextureRect(sf::IntRect(56, 56, 56, 56));
	chessmen_bishop_white_spr.setTexture(chessmen_txt);
	chessmen_bishop_white_spr.setTextureRect(sf::IntRect(112, 56, 56, 56));
	chessmen_pawn_white_spr.setTexture(chessmen_txt);
	chessmen_pawn_white_spr.setTextureRect(sf::IntRect(280, 56, 56, 56));
	chessmen_rook_white_spr.setTexture(chessmen_txt);
	chessmen_rook_white_spr.setTextureRect(sf::IntRect(0, 56, 56, 56));
	chessmen_queen_white_spr.setTexture(chessmen_txt);
	chessmen_queen_white_spr.setTextureRect(sf::IntRect(168, 56, 56, 56));
	chessmen_king_black_spr.setTexture(chessmen_txt);
	chessmen_king_black_spr.setTextureRect(sf::IntRect(224, 0, 56, 56));
	chessmen_knight_black_spr.setTexture(chessmen_txt);
	chessmen_knight_black_spr.setTextureRect(sf::IntRect(56, 0, 56, 56));
	chessmen_bishop_black_spr.setTexture(chessmen_txt);
	chessmen_bishop_black_spr.setTextureRect(sf::IntRect(112, 0, 56, 56));
	chessmen_pawn_black_spr.setTexture(chessmen_txt);
	chessmen_pawn_black_spr.setTextureRect(sf::IntRect(280, 0, 56, 56));
	chessmen_rook_black_spr.setTexture(chessmen_txt);
	chessmen_rook_black_spr.setTextureRect(sf::IntRect(0, 0, 56, 56));
	chessmen_queen_black_spr.setTexture(chessmen_txt);
	chessmen_queen_black_spr.setTextureRect(sf::IntRect(168, 0, 56, 56));

	//loading UI texture
	ui_back_txt.loadFromFile(assets_image + "back.png");
	ui_back_spr.setTexture(ui_back_txt);
	ui_forward_txt.loadFromFile(assets_image + "forward.png");
	ui_forward_spr.setTexture(ui_forward_txt);
	ui_save_txt.loadFromFile(assets_image + "save.png");
	ui_save_spr.setTexture(ui_save_txt);
	ui_load_txt.loadFromFile(assets_image + "load.png");
	ui_load_spr.setTexture(ui_load_txt);
	ui_retry_txt.loadFromFile(assets_image + "retry.png");
	ui_retry_spr.setTexture(ui_retry_txt);

	ui_elements.push_back(&ui_back_spr); ui_elements.push_back(&ui_forward_spr); ui_elements.push_back(&ui_save_spr); ui_elements.push_back(&ui_load_spr); ui_elements.push_back(&ui_retry_spr);
}

chessfield::game_status sfmlRenderer::processOutput(chessfield& game, chessfield::full_game_status status) {
	if (status == chessfield::next) {
		if (game.current_player == chessmen::white)
			game.current_player = chessmen::black;
		else
			game.current_player = chessmen::white;
		return chessfield::running;
	}
	else if (status == chessfield::error) {
		std::cout << "ERROR: clickfield() reached control path end without a valid state" << std::endl;
		return chessfield::end;
	}
	else if (status == chessfield::selected) {
		//std::cout << "A chessmen was successfully selected" << std::endl;
		return chessfield::running;
	}
	else if (status == chessfield::enemy) {
		std::cout << "Please select one of your chessmen" << std::endl;
		return chessfield::mistake;
	}
	else if (status == chessfield::emptyfield) {
		std::cout << "That field seems to be empty" << std::endl;
		return chessfield::mistake;
	}
	else if (status == chessfield::checked) {
		std::cout << "This move would result in you being checked" << std::endl;
		return chessfield::mistake;
	}
	else if (status == chessfield::impmove) {
		std::cout << "This move is not possible" << std::endl;
		return chessfield::mistake;
	}
	else if (status == chessfield::bkstale) {
		std::cout << "DRAW: the black king is stale" << std::endl;
		return chessfield::end;
	}
	else if (status == chessfield::wkstale) {
		std::cout << "DRAW: the white king is stale" << std::endl;
		return chessfield::end;
	}
	else if (status == chessfield::bkmate) {
		std::cout << "WHITE WINS: the black king is mate" << std::endl;
		return chessfield::end;
	}
	else if (status == chessfield::wkmate) {
		std::cout << "BLACK WINS: the white king is mate" << std::endl;
		return chessfield::end;
	}
	else {
		//std::cout << "An internal error occured :(" << std::endl;
		return chessfield::end;
	}
}

void sfmlRenderer::render(chessfield& game, sf::RenderWindow & window) {
	//draw UI
	if (game.current_player == chessmen::white)
		ui_elements.push_back(new sf::Sprite(chessmen_king_white_spr));
	else
		ui_elements.push_back(new sf::Sprite(chessmen_king_black_spr));

	sf::Vector2u chessboard_size = chessboard_txt.getSize();
	int ui_height = screenHeight - chessboard_size.y;
	int ui_width = screenWidth;
	ui_element_width = (ui_width / ui_elements.size()) - 16;

	for (size_t i = 0; i < ui_elements.size(); i++) {
		float sizex = ui_elements[i]->getTextureRect().width;
		float sizey = ui_elements[i]->getTextureRect().height;
		float scalex = (float)ui_element_width / sizex;
		float scaley = (float)ui_height / sizey;
		float posx = i * ui_element_width + 32;
		float posy = chessboard_size.y;
		ui_elements[i]->setColor(sf::Color::White);
		if (scalex < scaley)
			ui_elements[i]->setScale({ scalex, scalex });
		else
			ui_elements[i]->setScale({ scaley, scaley });
		ui_elements[i]->setPosition({posx, posy });
		window.draw(*ui_elements[i]);
	}

	delete ui_elements.back();
	ui_elements.pop_back();

	//draw game
	window.draw(chessboard_spr);

	if (game.selected_chessmen != nullptr) {
		sf::RectangleShape selected(sf::Vector2f(field_width, field_height));
		selected.setFillColor(sf::Color(25, 225, 0, 225));
		int posx = game.selected_chessmen->board_position.x * field_width + 28;
		int posy = game.selected_chessmen->board_position.y * field_height + 28;
		selected.setPosition(posx, posy);
		window.draw(selected);

		auto possibleMoves = game.truePossibleMoves(game.selected_chessmen, &game.chessmen_onfield);
		for (size_t i = 0; i < possibleMoves.size(); i++) {
			sf::RectangleShape pmindicator(sf::Vector2f(field_width, field_height));
			pmindicator.setFillColor(sf::Color(50, 100,0,225));
			int posx = possibleMoves[i].x * field_width + 28;
			int posy = possibleMoves[i].y * field_height + 28;
			pmindicator.setPosition(posx, posy);
			window.draw(pmindicator);
		}
	}

	for (size_t i = 0; i < game.chessmen_onfield.size(); i++) {
		sf::Sprite* current_sprite = nullptr;
		int posx = game.chessmen_onfield[i]->board_position.x * field_width + 28;
		int posy = game.chessmen_onfield[i]->board_position.y * field_height + 28;

		auto figure = game.chessmen_onfield[i]->figure();

		if (figure == chessmen::queen) {
			if (game.chessmen_onfield[i]->player_color == chessmen::black) {
				current_sprite = &chessmen_queen_black_spr;
			}
			else {
				current_sprite = &chessmen_queen_white_spr;
			}
		}
		else if (figure == chessmen::rook) {
			if (game.chessmen_onfield[i]->player_color == chessmen::black) {
				current_sprite = &chessmen_rook_black_spr;
			}
			else {
				current_sprite = &chessmen_rook_white_spr;
			}
		}
		else if (figure == chessmen::knight) {
			if (game.chessmen_onfield[i]->player_color == chessmen::black) {
				current_sprite = &chessmen_knight_black_spr;
			}
			else {
				current_sprite = &chessmen_knight_white_spr;
			}
		}
		else if (figure == chessmen::bishop) {
			if (game.chessmen_onfield[i]->player_color == chessmen::black) {
				current_sprite = &chessmen_bishop_black_spr;
			}
			else {
				current_sprite = &chessmen_bishop_white_spr;
			}
		}
		else if (figure == chessmen::pawn) {
			if (game.chessmen_onfield[i]->player_color == chessmen::black) {
				current_sprite = &chessmen_pawn_black_spr;
			}
			else {
				current_sprite = &chessmen_pawn_white_spr;
			}
		}
		else if (figure == chessmen::king) {
			if (game.chessmen_onfield[i]->player_color == chessmen::black) {
				current_sprite = &chessmen_king_black_spr;
			}
			else {
				current_sprite = &chessmen_king_white_spr;
			}
		}
		else {
			current_sprite = nullptr;
		}
		if (current_sprite != nullptr) {
			current_sprite->setPosition(posx, posy);
			window.draw(*current_sprite);
		}
		else {
			std::cout << std::endl;
		}
	}
}

bool sfmlRenderer::processUIInput(unsigned int ui_element, chessfield* game) {
	switch (ui_element) {
	case 0:
		game->stepback();
		return TRUE;
	default:
		return FALSE;
	}
}

int sfmlRenderer::gameLoop() {
	//creating window
	screenWidth = 500;
	screenHeight = 550;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), L"Chessgame (c) 2019 Constantin Fürst", sf::Style::Titlebar | sf::Style::Close);

	chessfield game;
	game.initGame();

	bool redraw = TRUE;
	bool lmb_press = FALSE;
	int drawpause = 0;

	while (window.isOpen()) {
		//sleep to save CPU cycles
		sleep(sf::milliseconds(1));

		//scale chessboard fields
		sf::Vector2u chessboard_size = chessboard_txt.getSize();
		chessboard_width = chessboard_size.x - (28 * 2);
		chessboard_height = chessboard_size.y - (28 * 2);
		field_height = chessboard_height / 8;
		field_width = chessboard_width / 8;

		bool paused = FALSE;

		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				std::cout << "paused" << std::endl;
				paused = TRUE;
			}
			else if (event.type == sf::Event::GainedFocus) {
				std::cout << "resumed" << std::endl;
				paused = FALSE;
			}
			else {
				if (paused == TRUE) {
					sleep(sf::seconds(1));
					window.clear();
					window.display();
				}
				else {
					break;
				}
			}
		}

		if (paused == FALSE) {
			//get mouse input
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				lmb_press = TRUE;
			}
			else if (lmb_press == TRUE) {
				const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				const unsigned int clickedX = (mousePosition.x - 28) / field_width;
				const unsigned int clickedY = (mousePosition.y - 28) / field_height;
				const chessmen::position clickedPOS = { clickedX, clickedY };
				if (chessmen::validpos(clickedPOS)) {
					if (game.selected_chessmen != nullptr && clickedX == game.selected_chessmen->board_position.x && clickedY == game.selected_chessmen->board_position.y) {
						game.selected_chessmen = nullptr;
					}
					else {
						chessfield::full_game_status returnval = game.clickfield(clickedPOS, game.current_player);
						processOutput(game, returnval);
					}
					redraw = TRUE;
				}
				else {
					if (mousePosition.y < screenHeight && mousePosition.y > chessboard_height) {
						const unsigned int clickedUI = mousePosition.x / ui_element_width;
						if (processUIInput(clickedUI, &game) == TRUE)
							redraw = TRUE;
						std::cout << "UI element " << clickedUI << " clicked" << std::endl;
					}
				}
				lmb_press = FALSE;
			}
			else {
				lmb_press = FALSE;
			}

			if (redraw == TRUE) {
				//draw
				window.clear();
				render(game, window);
				window.display();
				redraw = FALSE;
			}
		}
	}
	return TRUE;
}