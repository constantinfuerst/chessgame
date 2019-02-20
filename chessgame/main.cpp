#include "pch.h"
#include "consoleRenderer.h"

int main () {
	chessfield game;
	while (TRUE) {
		game.initGame();
		chessfield::game_status game_status = chessfield::running;
		chessmen::color current_player = chessmen::white;
		render_field(game);

		while (game_status != chessfield::end) {
			bool newmove = FALSE;
			while (newmove == FALSE) {
				std::cout << (current_player == chessmen::white ? "its white's turn" : "its black's turn") << std::endl << "Please select a position" << std::endl;
				while (TRUE) {
					std::string selection;
					getline(std::cin, selection);
					if (validpos(strtopos(selection))) {
						if (game.clickfield(strtopos(selection), current_player) == chessfield::selected) {
							render_field(game);
							break;
						}
					}
				}
				std::cout << (current_player == chessmen::white ? "white" : "black") << " selected " << postostr(game.selected_chessmen[0]->current_position) << std::endl << "enter \"back\" to return to the select option or enter a position to move" << std::endl;
				while (TRUE) {
					std::string selection;
					getline(std::cin, selection);
					if (selection == "back") {
						game.selected_chessmen.clear();
						render_field(game);
						break;
					}
					else if (validpos(strtopos(selection))) {
						if (game.clickfield(strtopos(selection), current_player) == chessfield::running) {
							render_field(game);
							if (current_player == chessmen::white)
								current_player = chessmen::black;
							else
								current_player = chessmen::white;
							newmove = TRUE;
							break;
						}
					}
				}
			}
		}
	}
}