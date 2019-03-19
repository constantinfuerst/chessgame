#include "pch.h"
#include "consoleRenderer.h"

cg::position consoleRenderer::strtopos (std::string input) {
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

std::string consoleRenderer::postostr(cg::position pos) {
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

		if (i->figure() == cg::queen)
			displayBoard[y][x] = 'Q';
		else if (i->figure() == cg::rook)
			displayBoard[y][x] = 'R';
		else if (i->figure() == cg::knight)
			displayBoard[y][x] = 'N';
		else if (i->figure() == cg::bishop)
			displayBoard[y][x] = 'B';
		else if (i->figure() == cg::pawn)
			displayBoard[y][x] = 'P';
		else if (i->figure() == cg::king)
			displayBoard[y][x] = 'K';
		else
			displayBoard[y][x] = ' ';
		if (i->getPlayer() == cg::black)
			displayBoard[y][x + 1] = 'B';
		else if (i->getPlayer() == cg::white)
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

cg::game_status consoleRenderer::processOutput(cg::full_game_status status) {
	if (status == cg::next) {
		//std::cout << "Next player" << std::endl;
		return cg::running;
	}
	else if (status == cg::error) {
		std::cout << "ERROR: clickfield() reached control path end without a valid state" << std::endl;
		return cg::end;
	}
	else if (status == cg::selected) {
		//std::cout << "A chessmen was successfully selected" << std::endl;
		return cg::running;
	}
	else if (status == cg::enemy) {
		std::cout << "Please select one of your chessmen" << std::endl;
		return cg::mistake;
	}
	else if (status == cg::emptyfield) {
		std::cout << "That field seems to be empty" << std::endl;
		return cg::mistake;
	}
	else if (status == cg::checked) {
		std::cout << "This move would result in you being checked" << std::endl;
		return cg::mistake;
	}
	else if (status == cg::impmove) {
		std::cout << "This move is not possible" << std::endl;
		return cg::mistake;
	}
	else if (status == cg::bkstale) {
		std::cout << "DRAW: the black king is stale" << std::endl;
		return cg::end;
	}
	else if (status == cg::wkstale) {
		std::cout << "DRAW: the white king is stale" << std::endl;
		return cg::end;
	}
	else if (status == cg::bkmate) {
		std::cout << "WHITE WINS: the black king is mate" << std::endl;
		return cg::end;
	}
	else if (status == cg::wkmate) {
		std::cout << "BLACK WINS: the white king is mate" << std::endl;
		return cg::end;
	}
	else {
		//std::cout << "An internal error occured :(" << std::endl;
		return cg::end;
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

		cg::game_status game_status = cg::running;
		render(game);

		while (TRUE) {
			while (TRUE) {
				std::cout << (game.current_player == cg::white ? "White" : "Black") << ", please select a position" << std::endl;
				getline(std::cin, selection);
				try {
					auto const moveFull = game.clickfield(strtopos(selection), game.current_player);
					auto const move = processOutput(moveFull);
					if (move == cg::running) {
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
				std::cout << (game.current_player == cg::white ? "White" : "Black") << " selected " << postostr(game.selected_chessmen->getPos()) << ", enter \"back\" to return or a position to move" << std::endl;
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
						if (move == cg::running) {
							render(game);
							if (game.current_player == cg::white)
								game.current_player = cg::black;
							else
								game.current_player = cg::white;
							break;
						}
						else if (move == cg::end) {
							render(game);
							processOutput(moveFull);
							game_status = cg::end;
							break;
						}
					}
				}
				catch (const std::exception& exception) {
					render(game);
					std::cout << "The entered position does not seem to be valid" << std::endl;
				}
			}
			if (game_status == cg::end) {
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
						else if (game.createSaveGame(selection, "consolesavegame") == FALSE) {
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