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

		//TODO: Implement a more sophisticated way of checking for a valid casteling move
		if (chessmen->figure() == chessmen::king) {
			auto player = chessmen->getPlayer();
			for (size_t i = 0; i < casteling(player).size(); i++) {
				int required_empty = 0;
				int empty = 0;
				chessmen::position position = casteling(player)[i].newkpos;
				if (position.x > chessmen->getPos().x && chessmen->getPlayer() == chessmen::white) {
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
				else if (position.x < chessmen->getPos().x && chessmen->getPlayer() == chessmen::black) {
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
				else if (position.x > chessmen->getPos().x && chessmen->getPlayer() == chessmen::black) {
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
				else if (position.x < chessmen->getPos().x && chessmen->getPlayer() == chessmen::white) {
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
	if (last_game_status >= 8) { //when the game is allready over return the last state
		return last_game_status;
	}
	else if (selected_chessmen == nullptr) {
		try {
			chessmen* clicked_chessmen = findChessmen(field);
			if (clicked_chessmen->getPlayer() != player) {
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
			selected_chessmen->setHasMoved(TRUE);
			//replacing pawn at the end of field with queen
			if (selected_chessmen->figure() == chessmen::pawn) {
				if (selected_chessmen->getPos().y == chessmen::fieldsize_y_start && selected_chessmen->getPlayer() == chessmen::black || selected_chessmen->getPos().y == chessmen::fieldsize_y_end && selected_chessmen->getPlayer() == chessmen::white) {
					chessmen::position pos = selected_chessmen->getPos();
					const chessmen::color col = selected_chessmen->getPlayer();
					movetoside(pos, &chessmen_onfield, &chessmen_onside, &changes, TRUE);
					newchessmen(pos, &changes, col, chessmen::queen, TRUE);
				}
			}
			selected_chessmen = nullptr;
			forwardmovetrace.clear();
			backwardmovetrace.push_back(std::make_unique<move>(changes));
			if (king_situation(chessmen::black) == stale) {
				last_game_status = bkstale;
				return bkstale;
			}
			else if (king_situation(chessmen::white) == stale) {
				last_game_status = wkstale;
				return wkstale;
			}
			else if (king_situation(chessmen::white) == checkmate) {
				last_game_status = wkmate;
				return wkmate;
			}
			else if (king_situation(chessmen::black) == checkmate) {
				last_game_status = bkmate;
				return bkmate;
			}
			else {
				return next;
			}
		}
	}
	return error;
}
