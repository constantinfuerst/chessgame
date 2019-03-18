#include "pch.h"
#include "consoleRenderer.h"

chessmen::position consoleRenderer::strtopos (std::string input) {
	if (input.size() != 2) {
		throw notfound();
	}
	unsigned int x = int(input[0]) - 65;
	const unsigned int y = std::stoi(input.substr(1, 1)) - 1;
	if (chessmen::validpos({ x, y })) {
		return { x, y };
	}
	else {
		x = int(input[0]) - 97;
		if (chessmen::validpos({ x, y })) {
			return { x, y };
		}
		throw notfound();
	}
}

std::string consoleRenderer::postostr(chessmen::position pos) {
	return char(pos.x + 65) + std::to_string(pos.y + 1);
}

unsigned int consoleRenderer::translateX (const unsigned int& org_x) {
	unsigned int x = 0;
	if (org_x == 0)
		x = 7;
	else if (org_x == 1)
		x = 14;
	else if (org_x == 2)
		x = 21;
	else if (org_x == 3)
		x = 28;
	else if (org_x == 4)
		x = 35;
	else if (org_x == 5)
		x = 42;
	else if (org_x == 6)
		x = 49;
	else if (org_x == 7)
		x = 56;
	else
		x = 0;
	return x;
}

unsigned int consoleRenderer::translateY(const unsigned int& org_y) {
	unsigned int y = 0;
	if (org_y == 0)
		y = 31;
	else if (org_y == 1)
		y = 27;
	else if (org_y == 2)
		y = 23;
	else if (org_y == 3)
		y = 19;
	else if (org_y == 4)
		y = 15;
	else if (org_y == 5)
		y = 11;
	else if (org_y == 6)
		y = 7;
	else if (org_y == 7)
		y = 3;
	return y;
}

void consoleRenderer::render(chessfield& board) {
	system("cls");
	char displayBoard[34][61] = {
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', 'B', ' ', ' ', ' ', ' ', ' ', ' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', 'D', ' ', ' ', ' ', ' ', ' ', ' ', 'E', ' ', ' ', ' ', ' ', ' ', ' ', 'F', ' ', ' ', ' ', ' ', ' ', ' ', 'G', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '8', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '7', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '6', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '5', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '4', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '3', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '2', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', '1', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
		{' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	};

	for (auto& i : *board.getField()) {
		const unsigned int x = translateX(i->getPos().x);
		const unsigned int y = translateY(i->getPos().y);

		if (i->figure() == chessmen::queen)
			displayBoard[y][x] = 'Q';
		else if (i->figure() == chessmen::rook)
			displayBoard[y][x] = 'R';
		else if (i->figure() == chessmen::knight)
			displayBoard[y][x] = 'N';
		else if (i->figure() == chessmen::bishop)
			displayBoard[y][x] = 'B';
		else if (i->figure() == chessmen::pawn)
			displayBoard[y][x] = 'P';
		else if (i->figure() == chessmen::king)
			displayBoard[y][x] = 'K';
		else
			displayBoard[y][x] = ' ';
		if (i->getPlayer() == chessmen::black)
			displayBoard[y][x + 1] = 'B';
		else if (i->getPlayer() == chessmen::white)
			displayBoard[y][x + 1] = 'W';
		else
			displayBoard[y][x + 1] = ' ';
	}

	if (board.selected_chessmen != nullptr) {
		auto possibleMoves = board.truePossibleMoves(board.selected_chessmen, board.getField());
		if (!possibleMoves.empty()) {
			for (auto& possibleMove : possibleMoves) {

				auto testx = possibleMove.x;
				auto testy = possibleMove.y;

				const unsigned int x = translateX(possibleMove.x);
				const unsigned int y = translateY(possibleMove.y);

				displayBoard[y + 1][x - 2] = char(177);
				displayBoard[y    ][x - 2] = char(177);
				displayBoard[y - 1][x - 2] = char(177);
				displayBoard[y + 1][x    ] = char(177);
				displayBoard[y - 1][x    ] = char(177);
				displayBoard[y + 1][x + 1] = char(177);
				displayBoard[y - 1][x - 1] = char(177);
				displayBoard[y + 1][x - 1] = char(177);
				displayBoard[y - 1][x + 1] = char(177);
				displayBoard[y + 1][x + 2] = char(177);
				displayBoard[y - 1][x + 2] = char(177);
				displayBoard[y + 1][x + 3] = char(177);
				displayBoard[y    ][x + 3] = char(177);
				displayBoard[y - 1][x + 3] = char(177);
			}
		}

		const unsigned int x = translateX(board.selected_chessmen->getPos().x);
		const unsigned int y = translateY(board.selected_chessmen->getPos().y);

		displayBoard[y + 1][x - 2] = char(192);
		displayBoard[y    ][x - 2] = char(179);
		displayBoard[y - 1][x - 2] = char(218);

		displayBoard[y + 1][x    ] = char(196);
		displayBoard[y - 1][x    ] = char(196);
		displayBoard[y + 1][x + 1] = char(196);
		displayBoard[y - 1][x - 1] = char(196);
		displayBoard[y + 1][x - 1] = char(196);
		displayBoard[y - 1][x + 1] = char(196);

		displayBoard[y + 1][x + 2] = char(196);
		displayBoard[y - 1][x + 2] = char(196);

		displayBoard[y + 1][x + 3] = char(217);
		displayBoard[y    ][x + 3] = char(179);
		displayBoard[y - 1][x + 3] = char(191);
	}

	for (auto i = 0; i < 34; i++) {
		std::cout << std::endl;
		for (int j = 0; j < 61; j++) {
			std::wcout << displayBoard[i][j];
		}
	}
	std::cout << std::endl << std::endl;
}

chessfield::game_status consoleRenderer::processOutput(chessfield::full_game_status status) {
	if (status == chessfield::next) {
		//std::cout << "Next player" << std::endl;
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

int consoleRenderer::gameLoop() const {
	chessfield game;
	std::string selection;
	while (TRUE) {
		while (TRUE) {
			bool loaded = FALSE;
			std::cout << R"(Would you like to load a savegame? Enter "yes" or "no")" << std::endl;
			getline(std::cin, selection);
			if (selection == "yes") {
				while (TRUE) {
					std::cout << "Please enter the name and directory of the savegame you want to load or enter \"return\" to return" << std::endl;
					getline(std::cin, selection);
					if (selection == "return") {
						break;
					}
					else if (game.initSaveGame(selection) == FALSE) {
						std::cout << "Something went wrong while reading from your savegame, please try that again" << std::endl;
					}
					else {
						loaded = TRUE;
						break;
					}
				}
			}
			else if (selection == "no") {
				game.initGame();
				loaded = TRUE;
				break;
			}
			else {
				std::cout << "Please try that again" << std::endl;
			}
			if (loaded == TRUE) {
				break;
			}
		}

		chessfield::game_status game_status = chessfield::running;
		render(game);

		while (TRUE) {
			while (TRUE) {
				std::cout << (game.current_player == chessmen::white ? "White" : "Black") << ", please select a position" << std::endl;
				getline(std::cin, selection);
				try {
					auto const moveFull = game.clickfield(strtopos(selection), game.current_player);
					auto const move = processOutput(moveFull);
					if (move == chessfield::running) {
						render(game);
						break;
					}
				}
				catch (const std::exception& exception) {
					render(game);
					std::cout << "The entered position does not seem to be valid" << std::endl;
				}
			}
			while (TRUE) {
				std::cout << (game.current_player == chessmen::white ? "White" : "Black") << " selected " << postostr(game.selected_chessmen->getPos()) << ", enter \"back\" to return or a position to move" << std::endl;
				getline(std::cin, selection);
				try {
					if (selection == "back") {
						render(game);
						game.selected_chessmen = nullptr;
						break;
					}
					else {
						auto const moveFull = game.clickfield(strtopos(selection), game.current_player);
						auto const move = processOutput(moveFull);
						if (move == chessfield::running) {
							render(game);
							if (game.current_player == chessmen::white)
								game.current_player = chessmen::black;
							else
								game.current_player = chessmen::white;
							break;
						}
						else if (move == chessfield::end) {
							render(game);
							processOutput(moveFull);
							game_status = chessfield::end;
							break;
						}
					}
				}
				catch (const std::exception& exception) {
					render(game);
					std::cout << "The entered position does not seem to be valid" << std::endl;
				}
			}
			if (game_status == chessfield::end) {
				break;
			}
		}

		std::cout << "Would you like to play another match? Enter enter \"quit\" to quit, press enter to rematch" << std::endl;
		getline(std::cin, selection);
		if (selection == "quit") {
			while (TRUE) {
				bool saved = FALSE;
				std::cout << R"(Would you like to save this game? Enter "yes" or "no")" << std::endl;
				getline(std::cin, selection);
				if (selection == "yes") {
					while (TRUE) {
						std::cout << "Please enter a directory and name for your savegame or \"return\" to return" << std::endl;
						getline(std::cin, selection);
						if (selection == "return") {
							break;
						}
						else if (game.createSaveGame(selection) == FALSE) {
							std::cout << "Something went wrong while writing your savegame, please try that again" << std::endl;
						}
						else {
							game.quit();
							saved = TRUE;
							break;
						}
					}
				}
				else if (selection == "no") {
					game.quit();
					saved = TRUE;
					break;
				}
				else {
					std::cout << "Please try that again" << std::endl;
				}
				if (saved == TRUE) {
					break;
				}
			}
			break;
		}
		break;
	}
	return TRUE;
}