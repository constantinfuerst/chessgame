#include "pch.h"
#include "sfmlRenderer.h"

#ifdef WIN_DESKTOP_GUI
	#define cout(str)
#else
	#include <iostream>
	#define cout(str) std::cout << str << std::endl;
#endif

sfmlRenderer::sfmlRenderer() {
	//loading assets
	sfmlRenderAsstes::get()->loadAssets();
	//creating window
	screen_dims.width = 500;
	screen_dims.height = 550;
	window = new sf::RenderWindow (sf::VideoMode(screen_dims.width, screen_dims.height), L"Chessgame (c) 2019 Constantin Fürst", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(10); //only update every 100ms
	gui = new tgui::Gui {*window};
	game = new chessfield;
	render();
}

sfmlRenderer::~sfmlRenderer() {
	delete gui;
	delete window;
	delete game;
}

void sfmlRenderer::processOutput(cg::full_game_status status) const {
	if (status == cg::next) {
		if (game->current_player == cg::white)
			game->current_player = cg::black;
		else
			game->current_player = cg::white;
	}
	else if (status == cg::error) {
		cout("error clickfield");
	}
	else if (status == cg::selected) {
		cout("selected");
	}
	else if (status == cg::enemy) {
		cout("enemy clicked");
	}
	else if (status == cg::emptyfield) {
		cout("emptyfield clicked");
	}
	else if (status == cg::checked) {
		cout("checked field clicked");
	}
	else if (status == cg::impmove) {
		cout("impmove clicked");
	}
	else if (status == cg::bkstale) {
		cout("bkstale");
		ui_child_create(cg::ui_newgame, "DRAW:\nthe black king is stale");
	}
	else if (status == cg::wkstale) {
		cout("wkstale");
		ui_child_create(cg::ui_newgame, "DRAW:\nthe white king is stale");
	}
	else if (status == cg::bkmate) {
		cout("bkmate");
		ui_child_create(cg::ui_newgame, "WHITE WINS:\nthe black king is mate");
	}
	else if (status == cg::wkmate) {
		cout("wkmate");
		ui_child_create(cg::ui_newgame, "BLACK WINS:\nthe white king is mate");
	}
	else {
		cout("interror processOutput");
	}
}

std::vector <std::string> sfmlRenderer::getFiles() {
	std::vector <std::string> returnval;
	returnval.reserve(25);
	for (const auto& file : std::experimental::filesystem::directory_iterator(SAVE_DIR)) {
		std::wstring_convert < std::codecvt_utf8_utf16 <wchar_t> > converter;
		std::string filename = converter.to_bytes(file.path());
		if (filename.find(".csg") != std::wstring::npos) {
			returnval.push_back(filename);
		}
	}
	return returnval;
}

void sfmlRenderer::createSavegame(tgui::EditBox::Ptr filename_box) const {
	const auto filename = filename_box->getText().toAnsiString();
	if (game->createSaveGame(SAVE_DIR + filename + ".csg", filename) == TRUE) {
		cout("created game state .csg file");
		ui_child_create(cg::ui_message, "Saving successfull!");
	}
	else {
		cout("failed creating game state .csg file");
		ui_child_create(cg::ui_message, "Trying to save the game failed!");
	}
}

void sfmlRenderer::loadSavegame(std::string filename) const {
	if (game->initSaveGame(filename) == TRUE) {
		cout("loaded game state .csg file");
		ui_child_create(cg::ui_message, "Loading successfull!");
	}
	else {
		cout("failed loading game state .csg file");
		ui_child_create(cg::ui_message, "Trying to load the game failed!");
	}
}

bool sfmlRenderer::processUIInput(unsigned int ui_element) const {
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
		ui_child_create(cg::ui_savegame);
		return TRUE;
	case 3:
		ui_child_create(cg::ui_loadgame);
		return TRUE;
	case 4:
		ui_child_create(cg::ui_newgame);
		return TRUE;
	default:
		return FALSE;
	}
}

int sfmlRenderer::gameLoop() {
	game_status = cg::running;
	game->initGame();

	displayingUI = cg::nodisplay;
	redraw = TRUE;

	while (window->isOpen()) {
		//sleep to save CPU cycles
		sleep(sf::milliseconds(1));

		//scale chessboard fields
		field_dims.height = sfmlRenderAsstes::get()->getBoarddims().height / 8;
		field_dims.width = sfmlRenderAsstes::get()->getBoarddims().width / 8;

		sf::Event event;

		while (window->pollEvent(event)) {
			gui->handleEvent(event);
			if (event.type == sf::Event::Closed && displayingUI == cg::nodisplay) {
				window->close();
			}
			if (event.type == sf::Event::LostFocus) {
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
					window->display();
				}
				else {
					break;
				}
			}
		}

		///////////////////////
		// actual Game Logic //
		///////////////////////

		gameLogic();
	}
	return TRUE;
}

void sfmlRenderer::gameLogic() {
	//check for game status and react
	if (game_status == cg::restart) {
		game_status = cg::running;
		game->initGame();
	}
	if (redraw == TRUE && displayingUI != cg::nodisplay) {
		window->clear();
		render();
		gui->draw();
		window->display();
	}
	if (displayingUI == cg::display_noupdate) {
		window->setFramerateLimit(15); //update every 32ms to make the fading button effect smooth
		//draw
		gui->draw();
	}
	else if (displayingUI == cg::display_update) {
		window->setFramerateLimit(60); //update every 16ms to make the text input smooth
		window->clear();
		render();
		gui->draw();
		window->display();
	}
	else if (paused == FALSE) {
		window->setFramerateLimit(10); //only update every 100ms
		//get mouse input
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			lmb_press = TRUE;
		}
		else if (lmb_press == TRUE && displayingUI == cg::nodisplay) {
			const sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
			const unsigned int clickedX = (mousePosition.x - 28) / field_dims.width;
			const unsigned int clickedY = (mousePosition.y - 28) / field_dims.height;
			const cg::position clickedPOS = { clickedX, clickedY };
			if (chessmen::validpos(clickedPOS)) {
				cout("clicked field x:" << clickedX << " y: " << clickedY);
				if (game->selected_chessmen != nullptr && clickedX == game->selected_chessmen->getPos().x && clickedY == game->selected_chessmen->getPos().y) {
					cout("unselecting chessmen");
					game->selected_chessmen = nullptr;
				}
				else {
					const cg::full_game_status returnval = game->clickfield(clickedPOS, game->current_player);
					cout("executing clickfield and processOutput");
					processOutput(returnval);
				}
				redraw = TRUE;
			}
			else {
				if (static_cast<unsigned>(mousePosition.y) < screen_dims.height && mousePosition.y > sfmlRenderAsstes::get()->getBoarddims().height) {
					const unsigned int clickedUI = mousePosition.x / ui_element_width;
					if (processUIInput(clickedUI) == TRUE)
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
			window->clear();
			render();
			gui->draw();
			window->display();
			redraw = FALSE;
		}
	}
}