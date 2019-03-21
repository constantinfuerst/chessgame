#include "pch.h"
#include "sfmlRenderer.h"

#ifdef WIN_DESKTOP_GUI
	#define cout(str)
#else
	#include <iostream>
	#define cout(str) std::cout << str << std::endl;
#endif

//creates window and makes the singleton load its assets
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

//function that handles output by the clickfield() function of the chessfield
void sfmlRenderer::processOutput(cg::full_game_status status) const {
	//if the next player should be selected do so
	if (status == cg::next) {
		if (game->current_player == cg::white)
			game->current_player = cg::black;
		else
			game->current_player = cg::white;
	}
	//an error occured, if we are in console-gui mode write a message
	else if (status == cg::error) {
		cout("error clickfield");
	}
	//a chesspiece was selected, if we are in console-gui mode write a message
	else if (status == cg::selected) {
		cout("selected");
	}
	//a chesspiece of the enemy was clicked, if we are in console-gui mode write a message
	else if (status == cg::enemy) {
		cout("enemy clicked");
	}
	//an empty field was clicked, if we are in console-gui mode write a message
	else if (status == cg::emptyfield) {
		cout("emptyfield clicked");
	}
	//the king would be check in this position, if we are in console-gui mode write a message
	else if (status == cg::checked) {
		cout("checked field clicked");
	}
	//that move is impossible, if we are in console-gui mode write a message
	else if (status == cg::impmove) {
		cout("impmove clicked");
	}
	//the black king is stale, create a GUI message
	else if (status == cg::bkstale) {
		cout("bkstale");
		ui_child_create(cg::ui_newgame, "DRAW:\nthe black king is stale");
	}
	//the white king is stale, create a GUI message
	else if (status == cg::wkstale) {
		cout("wkstale");
		ui_child_create(cg::ui_newgame, "DRAW:\nthe white king is stale");
	}
	//the black king is mate, create a GUI message
	else if (status == cg::bkmate) {
		cout("bkmate");
		ui_child_create(cg::ui_newgame, "WHITE WINS:\nthe black king is mate");
	}
	//the white king is mate, create a GUI message
	else if (status == cg::wkmate) {
		cout("wkmate");
		ui_child_create(cg::ui_newgame, "BLACK WINS:\nthe white king is mate");
	}
	//the handler cant process that, if we are in console-gui mode write a message
	else {
		cout("interror processOutput");
	}
}

//returns the full filepath of the savegames found in the "saves" folder
std::vector <std::string> sfmlRenderer::getFiles() {
	//create a vector, we expect no more that 25 games to be found
	std::vector <std::string> returnval;
	returnval.reserve(25);
	//iterate through the files in the savegames directory
	for (const auto& file : std::experimental::filesystem::directory_iterator(SAVE_DIR)) {
		std::wstring_convert < std::codecvt_utf8_utf16 <wchar_t> > converter;
		std::string filename = converter.to_bytes(file.path());
		//if the file has the ".csg" extension it must be a valid file
		if (filename.find(".csg") != std::wstring::npos) {
			returnval.push_back(filename);
		}
	}
	return returnval;
}

//function that reads the gamename from a tgui input box and creates a savegame with that name
void sfmlRenderer::createSavegame(tgui::EditBox::Ptr filename_box) const {
	const auto filename = filename_box->getText().toAnsiString();
	//if creating the savegame was successfull create a GUI message and if we are in console-gui mode write a message
	if (game->createSaveGame(SAVE_DIR + filename + ".csg", filename) == TRUE) {
		cout("created game state .csg file");
		ui_child_create(cg::ui_message, "Saving successfull!");
	}
	//if creating the savegame wasnt successfull create a GUI message and if we are in console-gui mode write a message
	else {
		cout("failed creating game state .csg file");
		ui_child_create(cg::ui_message, "Trying to save the game failed!");
	}
}

//function that loads a game from a specified file
void sfmlRenderer::loadSavegame(std::string filename) const {
	//if loading the savegame was successfull create a GUI message and if we are in console-gui mode write a message
	if (game->initSaveGame(filename) == TRUE) {
		cout("loaded game state .csg file");
		ui_child_create(cg::ui_message, "Loading successfull!");
	}
	//if loading the savegame wasnt successfull create a GUI message and if we are in console-gui mode write a message
	else {
		cout("failed loading game state .csg file");
		ui_child_create(cg::ui_message, "Trying to load the game failed!");
	}
}

//function that handles the users clicks on a UI element
bool sfmlRenderer::processUIInput(unsigned int ui_element) const {
	switch (ui_element) {
	case 0:
		//the backward-arrow was clicked
		game->stepback();
		game->selected_chessmen = nullptr;
		return TRUE;
	case 1:
		//the forward-arrow was clicked
		game->stepforward();
		game->selected_chessmen = nullptr;
		return TRUE;
	case 2:
		//the save-icon was clicked
		ui_child_create(cg::ui_savegame);
		return TRUE;
	case 3:
		//the load-icon was clicked
		ui_child_create(cg::ui_loadgame);
		return TRUE;
	case 4:
		//the newgame-icon was clicked
		ui_child_create(cg::ui_newgame);
		return TRUE;
	default:
		return FALSE;
	}
}

//the renderer loop and entry point of the smfl implementation
int sfmlRenderer::gameLoop() {
	//start up the game
	game_status = cg::running;
	game->initGame();

	displayingUI = cg::nodisplay;
	redraw = TRUE;

	//while the window is open
	while (window->isOpen()) {
		//sleep to save CPU cycles
		sleep(sf::milliseconds(1));

		//scale chessboard fields
		field_dims.height = sfmlRenderAsstes::get()->getBoarddims().height / 8;
		field_dims.width = sfmlRenderAsstes::get()->getBoarddims().width / 8;

		sf::Event event;

		#ifndef WIN_DESKTOP_GUI
		window->setTitle("mouse x: " + std::to_string(sf::Mouse::getPosition(*window).x) + " y: " + std::to_string(sf::Mouse::getPosition(*window).y));
		#endif

		//handle the events of the window
		while (window->pollEvent(event)) {
			gui->handleEvent(event);
			//when the window was closed quit the game and close the window
			if (event.type == sf::Event::Closed) {
				game->quit();
				window->close();
			}
			else {
				//when we lost focus (window is in the background) pause the game
				if (event.type == sf::Event::LostFocus) {
					cout("paused main game loop");
					paused = TRUE;
				}
				//when regaining focus resume and redraw the game
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
		}

		///////////////////////
		// actual Game Logic //
		///////////////////////

		gameLogic();
	}
	return TRUE;
}

void sfmlRenderer::gameLogic() {
	//if the game is supposed to be restarted do so
	if (game_status == cg::restart) {
		game_status = cg::running;
		game->initGame();
	}
	//if we are displaying static UI just display that UI, no need to update the screen
	if (displayingUI == cg::display_noupdate) {
		window->setFramerateLimit(15); //update every 32ms to make the fading button effect smooth
		gui->draw();
	}
	//if we are displaying dynamic UI display the UI and render the game (without allowing interaction with the game though
	else if (displayingUI == cg::display_update) {
		window->setFramerateLimit(60); //update every 16ms to make the text input smooth
		window->clear();
		render();
		gui->draw();
		window->display();
	}
	//if we are not displaying UI and the game isnt paused enter the "interaction"-phase
	else if (paused == FALSE) {
		window->setFramerateLimit(10); //only update every 100ms
		//get mouse input
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			lmb_press = TRUE;
		}
		//if the mouse button is pressed
		else if (lmb_press == TRUE) {
			//get the mouse position and translate that to clicked field
			const sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
			const cg::position clicked = {
				(mousePosition.x - 28) / field_dims.width,
				(mousePosition.y - 28) / field_dims.height
			};
			//if the translated position is a position on the board
			if (chessmen::validpos(clicked)) {
				cout("clicked field x:" << clicked.x << " y: " << clicked.y);
				//if a chessmen is selected and the user clicked on that chessmen unselect it
				if (game->selected_chessmen != nullptr && clicked.x == game->selected_chessmen->getPos().x && clicked.y == game->selected_chessmen->getPos().y) {
					cout("unselecting chessmen");
					game->selected_chessmen = nullptr;
				}
				else {
					//use the clickfield event of the chessfield class to interact with the game
					const cg::full_game_status returnval = game->clickfield(clicked, game->current_player);
					cout("executing clickfield and processOutput");
					//now process the output of clickfield
					processOutput(returnval);
				}
				redraw = TRUE;
			}
			//if no boardposition was clicked it must be a UI interaction
			else {
				//check if the UI was actually clicked
				if (static_cast<unsigned>(mousePosition.y) < screen_dims.height && mousePosition.y > sfmlRenderAsstes::get()->getBoarddims().height) {
					//calculate the clicked UI element
					const unsigned int clickedUI = mousePosition.x / ui_element_width;
					//process the UI input and if that was successfull redraw to display the UI
					if (processUIInput(clickedUI) == TRUE)
						redraw = TRUE;
					cout("UI element " << clickedUI << " clicked");
				}
			}
			//now set the pressed state to false to only process one click
			lmb_press = FALSE;
		}
		else {
			lmb_press = FALSE;
		}
		//if the game should be rendered then do so
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