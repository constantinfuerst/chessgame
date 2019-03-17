#include "pch.h"
#include "sfmlRenderer.h"
#include "settings.h"

#ifdef WIN_DESKTOP_GUI
	#define cout(str)
#else
	#include <iostream>
	#define cout(str) std::cout << str << std::endl;
#endif

sfmlRenderer::sfmlRenderer() {
	render(nullptr, nullptr);
}

chessfield::game_status sfmlRenderer::processOutput(chessfield& game, chessfield::full_game_status status, tgui::Gui* gui) {
	if (status == chessfield::next) {
		if (game.current_player == chessmen::white)
			game.current_player = chessmen::black;
		else
			game.current_player = chessmen::white;
		return chessfield::running;
	}
	else if (status == chessfield::error) {
		cout("error clickfield");
		return chessfield::end;
	}
	else if (status == chessfield::selected) {
		cout("selected");
		return chessfield::running;
	}
	else if (status == chessfield::enemy) {
		//TODO: implement UI version
		cout("enemy clicked");
		return chessfield::mistake;
	}
	else if (status == chessfield::emptyfield) {
		//TODO: implement UI version
		cout("emptyfield clicked");
		return chessfield::mistake;
	}
	else if (status == chessfield::checked) {
		//TODO: implement UI version
		cout("checked field clicked");
		return chessfield::mistake;
	}
	else if (status == chessfield::impmove) {
		//TODO: implement UI version
		cout("impmove clicked");
		return chessfield::mistake;
	}
	else if (status == chessfield::bkstale) {
		cout("bkstale");
		ui_newgame(gui, "DRAW:\nthe black king is stale");
		return chessfield::end;
	}
	else if (status == chessfield::wkstale) {
		cout("wkstale");
		ui_newgame(gui, "DRAW:\nthe white king is stale");
		return chessfield::end;
	}
	else if (status == chessfield::bkmate) {
		cout("bkmate");
		ui_newgame(gui, "WHITE WINS:\nthe black king is mate");
		return chessfield::end;
	}
	else if (status == chessfield::wkmate) {
		cout("wkmate");
		ui_newgame(gui, "BLACK WINS:\nthe white king is mate");
		return chessfield::end;
	}
	else {
		cout("interror processOutput");
		return chessfield::end;
	}
}

bool sfmlRenderer::createSavegame(chessfield* game, tgui::Gui* gui) const {
	while (TRUE) {
		cout("Please enter a name for your savegame, enter \"back\" to return");
		std::string input;
		//getline(std::cin, input);
		if (game->createSaveGame(input) == TRUE) {
			return TRUE;
		}
		else if (input == "back") {
			return FALSE;
		}
		else {
			cout("Something went wrong, please try that again");
		}
	}
}

bool sfmlRenderer::loadSavegame(chessfield* game, tgui::Gui* gui) const {
	while (TRUE) {
		cout("Please enter a name for your savegame, enter \"back\" to return");
		std::string input;
		//getline(std::cin, input);
		if (game->initSaveGame(input) == TRUE) {
			return TRUE;
		}
		else if (input == "back") {
			return FALSE;
		}
		else {
			cout("Something went wrong, please try that again");
		}
	}
}

bool sfmlRenderer::processUIInput(unsigned int ui_element, chessfield* game, tgui::Gui* gui) {
	switch (ui_element) {
	case 0:
		game->stepback();
		game->selected_chessmen = nullptr;
		return TRUE;
	case 1:
		game->stepforward();
		game->selected_chessmen = nullptr;
		return TRUE;
	case 2:
		//TODO: implement UI version
		return createSavegame(game, gui);
	case 3:
		//TODO: implement UI version
		return loadSavegame(game, gui);
	case 4:
		ui_newgame(gui, "Do you really want\nto start a new game?");
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
	window.setFramerateLimit(10); //only update every 100ms
	tgui::Gui gui{window};
	chessfield game;
	game_status = chessfield::running;
	game.initGame();

	bool lmb_press = FALSE;
	displayingUI = FALSE;
	redraw = TRUE;

	while (window.isOpen()) {
		//sleep to save CPU cycles
		sleep(sf::milliseconds(1));

		//scale chessboard fields
		field_height = chessboard_height / 8;
		field_width = chessboard_width / 8;

		bool paused = FALSE;

		sf::Event event;

		while (window.pollEvent(event)) {
			gui.handleEvent(event);
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				cout("paused main game loop");
				paused = TRUE;
			}
			else if (event.type == sf::Event::GainedFocus) {
				cout("resumed main game loop");
				redraw = TRUE;
				paused = FALSE;
			}
			else {
				if (paused == TRUE) {
					window.display();
				}
				else {
					break;
				}
			}
		}

		//check for game status and react
		if (game_status == chessfield::restart) {
			game_status = chessfield::running;
			game.initGame();
		}

		if (displayingUI == TRUE) {
			//draw
			gui.draw();
		}
		else if (paused == FALSE) {
			//get mouse input
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				lmb_press = TRUE;
			}
			else if (lmb_press == TRUE && displayingUI == FALSE) {
				const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				const unsigned int clickedX = (mousePosition.x - 28) / field_width;
				const unsigned int clickedY = (mousePosition.y - 28) / field_height;
				const chessmen::position clickedPOS = { clickedX, clickedY };
				if (chessmen::validpos(clickedPOS)) {
					cout("clicked field x:" << clickedX << " y: " << clickedY);
					if (game.selected_chessmen != nullptr && clickedX == game.selected_chessmen->board_position.x && clickedY == game.selected_chessmen->board_position.y) {
						cout("unselecting chessmen");
						game.selected_chessmen = nullptr;
					}
					else {
						chessfield::full_game_status returnval = game.clickfield(clickedPOS, game.current_player);
						cout("executing clickfield and processOutput");
						processOutput(game, returnval, &gui);
					}
					redraw = TRUE;
				}
				else {
					if (mousePosition.y < screenHeight && mousePosition.y > chessboard_height) {
						const unsigned int clickedUI = mousePosition.x / ui_element_width;
						if (processUIInput(clickedUI, &game, &gui) == TRUE)
							redraw = TRUE;
						cout("UI element " << clickedUI << " clicked");
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
				render(&game, &window);
				gui.draw();
				window.display();
				redraw = FALSE;
			}
			else {
				continue;
			}
		}
		else {
			continue;
		}
	}
	return TRUE;
}

void sfmlRenderer::ui_newgame(tgui::Gui* gui, std::string message) {
	static tgui::Theme theme{ R"(C:\Users\Constantin\source\repos\chessgame\assets\guirenderer\TransparentGrey.txt)" };
	static const unsigned int width = 200;
	static const unsigned int height = 100;
	static const unsigned int posx = (screenWidth / 2) - (width / 2);
	static const unsigned int posy = ((screenHeight / 2) - 50) - (height / 2);
	static const unsigned int buttonwidth = width / 2;
	static const unsigned int buttonheight = height / 4;

	static bool constructed = FALSE;

	static auto child = tgui::ChildWindow::create();
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto nobutton = tgui::Button::create();

	if (displayingUI == TRUE) {
		return;
	}
	else if (constructed == TRUE) {
		label->setText(message);
		if (child->isVisible()) {
			cout("setting newgame_ui visible FALSE");
			child->setVisible(FALSE);
			displayingUI = FALSE;
		}
		else {
			cout("setting newgame_ui visible TRUE");
			child->setVisible(TRUE);
			displayingUI = TRUE;
		}
	}
	else {
		cout("constructing ui_newgame");

		constructed = TRUE;
		child->setResizable(FALSE);
		child->setPositionLocked(TRUE);
		child->setRenderer(theme.getRenderer("ChildWindow"));
		child->setSize(width, height);
		child->setPosition(posx, posy);
		child->setTitle("Confirm");
		child->connect("closed", [&]() {
			cout("child closed");
			displayingUI = FALSE;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		gui->add(child);
		label->setRenderer(theme.getRenderer("Label"));
		label->setText(message);
		label->setPosition(30, 10);
		label->setTextSize(15);
		child->add(label);
		yesbutton->setRenderer(theme.getRenderer("Button"));
		yesbutton->setPosition(0, height - buttonheight);
		yesbutton->setText("yes newgame");
		yesbutton->setSize(buttonwidth, buttonheight);
		yesbutton->connect("pressed", [&]() {
			cout("yes");
			game_status = chessfield::restart;
			displayingUI = FALSE;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		child->add(yesbutton);
		nobutton->setRenderer(theme.getRenderer("Button"));
		nobutton->setPosition(buttonwidth, height - buttonheight);
		nobutton->setText("no newgame");
		nobutton->setSize(buttonwidth, buttonheight);
		nobutton->connect("pressed", [&]() {
			cout("no");
			displayingUI = FALSE;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		child->add(nobutton);
	}
}

void sfmlRenderer::render(chessfield* game, sf::RenderWindow* window) {
	assets_image = R"(C:\Users\Constantin\source\repos\chessgame\assets\images\)";
	assets_font = R"(C:\Users\Constantin\source\repos\chessgame\assets\fonts\)";

	static bool constructed = FALSE;
	//loading board texture
	static sf::Texture chessboard_txt;
	static sf::Sprite chessboard_spr;
	//loading chessmen textures
	static sf::Texture chessmen_txt;
	//asigning sprites
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

	if (constructed == FALSE) {
		constructed = TRUE;
		cout("constructing renderer assets");

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

		sf::Vector2u chessboard_size = chessboard_txt.getSize();
		chessboard_width = chessboard_size.x - (28 * 2);
		chessboard_height = chessboard_size.y - (28 * 2);

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
	else {
		//draw UI
		if (game->current_player == chessmen::white)
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
			float scalex = static_cast<float>(ui_element_width) / sizex;
			float scaley = static_cast<float>(ui_height) / sizey;
			float posx = i * ui_element_width + 32;
			float posy = chessboard_size.y;
			ui_elements[i]->setColor(sf::Color::White);
			if (scalex < scaley)
				ui_elements[i]->setScale({ scalex, scalex });
			else
				ui_elements[i]->setScale({ scaley, scaley });
			ui_elements[i]->setPosition({ posx, posy });
			window->draw(*ui_elements[i]);
		}

		delete ui_elements.back();
		ui_elements.pop_back();

		//draw game
		window->draw(chessboard_spr);

		if (game->selected_chessmen != nullptr) {
			sf::RectangleShape selected(sf::Vector2f(field_width, field_height));
			selected.setFillColor(sf::Color(25, 225, 0, 225));
			int posx = game->selected_chessmen->board_position.x * field_width + 28;
			int posy = game->selected_chessmen->board_position.y * field_height + 28;
			selected.setPosition(posx, posy);
			window->draw(selected);

			auto possibleMoves = game->truePossibleMoves(game->selected_chessmen, &game->chessmen_onfield);
			for (auto& possibleMove : possibleMoves) {
				sf::RectangleShape pmindicator(sf::Vector2f(field_width, field_height));
				pmindicator.setFillColor(sf::Color(50, 100, 0, 225));
				int posx = possibleMove.x * field_width + 28;
				int posy = possibleMove.y * field_height + 28;
				pmindicator.setPosition(posx, posy);
				window->draw(pmindicator);
			}
		}

		for (auto& i : game->chessmen_onfield) {
			sf::Sprite* current_sprite = nullptr;
			int posx = i->board_position.x * field_width + 28;
			int posy = i->board_position.y * field_height + 28;

			auto figure = i->figure();

			if (figure == chessmen::queen) {
				if (i->player_color == chessmen::black) {
					current_sprite = &chessmen_queen_black_spr;
				}
				else {
					current_sprite = &chessmen_queen_white_spr;
				}
			}
			else if (figure == chessmen::rook) {
				if (i->player_color == chessmen::black) {
					current_sprite = &chessmen_rook_black_spr;
				}
				else {
					current_sprite = &chessmen_rook_white_spr;
				}
			}
			else if (figure == chessmen::knight) {
				if (i->player_color == chessmen::black) {
					current_sprite = &chessmen_knight_black_spr;
				}
				else {
					current_sprite = &chessmen_knight_white_spr;
				}
			}
			else if (figure == chessmen::bishop) {
				if (i->player_color == chessmen::black) {
					current_sprite = &chessmen_bishop_black_spr;
				}
				else {
					current_sprite = &chessmen_bishop_white_spr;
				}
			}
			else if (figure == chessmen::pawn) {
				if (i->player_color == chessmen::black) {
					current_sprite = &chessmen_pawn_black_spr;
				}
				else {
					current_sprite = &chessmen_pawn_white_spr;
				}
			}
			else if (figure == chessmen::king) {
				if (i->player_color == chessmen::black) {
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
				window->draw(*current_sprite);
			}
			else {
				cout("nullptr draw");
			}
		}
	}
}