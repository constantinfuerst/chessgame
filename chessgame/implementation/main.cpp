#include "pch.h"
#include "consoleRenderer.h"

int main () {
	chessfield game;
	while (TRUE) {
		game.initGame();
		chessfield::game_status game_status = chessfield::running;
		chessmen::color current_player = chessmen::white;
		render_field(game);

		while (TRUE) {
			bool newmove = FALSE;
			while (newmove == FALSE && game_status != chessfield::end) {
				while (TRUE) {
					std::cout << (current_player == chessmen::white ? "White" : "Black") << ", please select a position" << std::endl;
					std::string selection;
					getline(std::cin, selection);
					try {
						auto const move = processOutput(game.clickfield(strtopos(selection), current_player), game);
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
					std::cout << (current_player == chessmen::white ? "White" : "Black") << " selected " << postostr(game.selected_chessmen[0]->current_position) << ", enter \"back\" to return or a position to move" << std::endl;
					std::string selection;
					getline(std::cin, selection);
					try {
						if (selection == "back") {
							game.selected_chessmen.clear();
							render_field(game);
							break;
						}
						else {
							auto const move = processOutput(game.clickfield(strtopos(selection), current_player), game);
							if (move == chessfield::running) {
								render_field(game);
								if (current_player == chessmen::white)
									current_player = chessmen::black;
								else
									current_player = chessmen::white;
								newmove = TRUE;
								break;
							}
							else if (move == chessfield::end) {
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
			}
		}
	}
}
