#include "pch.h"
#include "chessfield.h"

std::vector<chessmen::position> chessfield::truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate) {
	std::vector<chessmen::position> returnpos = chessmen->possibleMoves(chessboard);
	bool clear = FALSE;
	if (dontCheckMate != TRUE) {
		if (selected_chessmen == nullptr) {
			clear = TRUE;
			selected_chessmen = chessmen;
		}

		size_t size = returnpos.size();
		size_t count = 0;
		while (count < size) {
			if (moveCharacter(returnpos[count], nullptr, onlytheoretical) == wouldbecheck) {
				returnpos.erase(returnpos.begin() + count);
				size = returnpos.size();
			}
			else {
				count++;
			}
		}

		if (clear == TRUE) {
			selected_chessmen = nullptr;
		}

		if (chessmen->figure() == chessmen::king) {
			for (size_t i = 0; i < casteling(chessmen->player_color).size(); i++) {
				int required_empty = 0;
				int empty = 0;
				chessmen::position position = std::get<0>(casteling(chessmen->player_color)[i]);
				if (position.x > chessmen->board_position.x && chessmen->player_color == chessmen::white) {
					chessmen::position secondpos = { position.x - 1, position.y };
					chessmen::position thrdpos = { position.x - 2, position.y };
					required_empty = 3;
					try {
						findChessmen(position);
					}
					catch (const std::exception& exception) {
						empty++;
					}
					try {
						findChessmen(secondpos);
					}
					catch (const std::exception& exception) {
						empty++;
					}
					try {
						findChessmen(thrdpos);
					}
					catch (const std::exception& exception) {
						empty++;
					}
				}
				else if (position.x < chessmen->board_position.x && chessmen->player_color == chessmen::black) {
					chessmen::position secondpos = { position.x + 1, position.y };
					chessmen::position thrdpos = { position.x + 2, position.y };
					required_empty = 3;
					try {
						findChessmen(position);
					}
					catch (const std::exception& exception) {
						empty++;
					}
					try {
						findChessmen(secondpos);
					}
					catch (const std::exception& exception) {
						empty++;
					}
					try {
						findChessmen(thrdpos);
					}
					catch (const std::exception& exception) {
						empty++;
					}
				}
				else if (position.x > chessmen->board_position.x && chessmen->player_color == chessmen::black) {
					chessmen::position secondpos = { position.x - 1, position.y };
					required_empty = 2;
					try {
						findChessmen(position);
					}
					catch (const std::exception& exception) {
						empty++;
					}
					try {
						findChessmen(secondpos);
					}
					catch (const std::exception& exception) {
						empty++;
					}
				}
				else if (position.x < chessmen->board_position.x && chessmen->player_color == chessmen::white) {
					chessmen::position secondpos = { position.x + 1, position.y };
					required_empty = 2;
					try {
						findChessmen(position);
					}
					catch (const std::exception& exception) {
						empty++;
					}
					try {
						findChessmen(secondpos);
					}
					catch (const std::exception& exception) {
						empty++;
					}
				}
				else {
					required_empty = 0;
					continue;
				}
				if (empty == required_empty) {
					returnpos.push_back(position);
				}
				else {
					continue;
				}
			}
		}
	}

	return returnpos;
}

chessfield::full_game_status chessfield::clickfield(chessmen::position field, chessmen::color player) {
	if (selected_chessmen == nullptr) {
		try {
			chessmen* clicked_chessmen = findChessmen(field);
			if (clicked_chessmen->player_color != player) {
				return enemy;
			}
			else {
				selected_chessmen = clicked_chessmen;
				return selected;
			}
		}
		catch (const std::exception& exception) {
			return emptyfield;
		}
	}
	else {
		move changes;
		changes.current_player = current_player;
		const auto result = moveCharacter(field, &changes, oncetheoretical);
		if (result != sucess) {
			if (result == wouldbecheck) {
				return checked;
			}
			else if (result == impossible) {
				return impmove;
			}
		}
		else {
			selected_chessmen->hasMoved = TRUE;
			//replacing pawn at the end of field with queen
			if (selected_chessmen->figure() == chessmen::pawn) {
				if (selected_chessmen->board_position.y == chessmen::fieldsize_y_start && selected_chessmen->player_color == chessmen::black || selected_chessmen->board_position.y == chessmen::fieldsize_y_end && selected_chessmen->player_color == chessmen::white) {
					chessmen::position pos = selected_chessmen->board_position;
					const chessmen::color col = selected_chessmen->player_color;
					movetoside(selected_chessmen->board_position, &chessmen_onfield, &chessmen_onside, &changes, TRUE);
					newchessmen(pos, &changes, col, chessmen::queen, TRUE);
				}
			}
			selected_chessmen = nullptr;
			forwardmovetrace.clear();
			std::cout << "clear" << std::endl;
			backwardmovetrace.push_back(std::unique_ptr<move>(new move(changes)));
			if (king_situation(chessmen::black) == stale) {
				return bkstale;
			}
			else if (king_situation(chessmen::white) == stale) {
				return wkstale;
			}
			else if (king_situation(chessmen::white) == checkmate) {
				return wkmate;
			}
			else if (king_situation(chessmen::black) == checkmate) {
				return bkmate;
			}
			else {
				return next;
			}
		}
	}
	return error;
}
