#include "pch.h"
#include "consoleRenderer.cpp"

int main () {
	chessfield game;
	while (TRUE) {
		game.initGame(chessmen::white);
		chessfield::game_status game_status = chessfield::running;
		render_field(game);

		while (TRUE) {
			while (TRUE) {
				std::cout << (game.current_player == chessmen::white ? "White" : "Black") << ", please select a position" << std::endl;
				std::string selection;
				getline(std::cin, selection);
				try {
					auto const moveFull = game.clickfield(strtopos(selection), game.current_player);
					auto const move = processOutput(moveFull);
					if (move == chessfield::running) {
						render_field(game);
						break;
					}
				}
				catch (const std::exception& exception) {
					render_field(game);
					std::cout << "The entered position does not seem to be valid" << std::endl;
				}
			}
			while (TRUE) {
				std::cout << (game.current_player == chessmen::white ? "White" : "Black") << " selected " << postostr(game.selected_chessmen->current_position) << ", enter \"back\" to return or a position to move" << std::endl;
				std::string selection;
				getline(std::cin, selection);
				try {
					if (selection == "back") {
						render_field(game);
						game.selected_chessmen = nullptr;
						break;
					}
					else {
						auto const moveFull = game.clickfield(strtopos(selection), game.current_player);
						auto const move = processOutput(moveFull);
						if (move == chessfield::running) {
							render_field(game);
							if (game.current_player == chessmen::white)
								game.current_player = chessmen::black;
							else
								game.current_player = chessmen::white;
							break;
						}
						else if (move == chessfield::end) {
							render_field(game);
							processOutput(moveFull);
							game_status = chessfield::end;
							break;
						}
					}
				}
				catch (const std::exception& exception) {
					render_field(game);
					std::cout << "The entered position does not seem to be valid" << std::endl;
				}
			}
			if (game_status == chessfield::end) {
				break;
			}
		}
		std::cout << "Would you like to play another match? Enter enter \"quit\" to quit, press enter to rematch" << std::endl;
		std::string selection;
		getline(std::cin, selection);
		if (selection == "quit") {
			break;
		}
		break;
	}
	std::cin.get();
	return TRUE;
}
