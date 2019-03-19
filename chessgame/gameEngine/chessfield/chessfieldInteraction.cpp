#include "pch.h"
#include "../cg_defs.h"
#include "chessfield.h"

std::vector<cg::position> chessfield::truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate) {
	std::vector<cg::position> returnpos = chessmen->possibleMoves(chessboard);
	bool clear = FALSE;
	if (dontCheckMate != TRUE) {
		if (selected_chessmen == nullptr) {
			clear = TRUE;
			selected_chessmen = chessmen;
		}

		size_t size = returnpos.size();
		size_t count = 0;
		while (count < size) {
			if (moveCharacter(returnpos[count], nullptr, cg::onlytheoretical) == cg::wouldbecheck) {
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
		#pragma warning(disable:4101)
		if (chessmen->figure() == cg::king) {
			auto player = chessmen->getPlayer();
			for (size_t i = 0; i < casteling(player).size(); i++) {
				int required_empty = 0;
				int empty = 0;
				cg::position position = casteling(player)[i].newkpos;
				if (position.x > chessmen->getPos().x && chessmen->getPlayer() == cg::white) {
					cg::position secondpos = { position.x - 1, position.y };
					cg::position thrdpos = { position.x - 2, position.y };
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
				else if (position.x < chessmen->getPos().x && chessmen->getPlayer() == cg::black) {
					cg::position secondpos = { position.x + 1, position.y };
					cg::position thrdpos = { position.x + 2, position.y };
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
				else if (position.x > chessmen->getPos().x && chessmen->getPlayer() == cg::black) {
					cg::position secondpos = { position.x - 1, position.y };
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
				else if (position.x < chessmen->getPos().x && chessmen->getPlayer() == cg::white) {
					cg::position secondpos = { position.x + 1, position.y };
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

cg::full_game_status chessfield::clickfield(cg::position field, cg::color player) {
	if (last_game_status >= 8) { //when the game is allready over return the last state
		return last_game_status;
	}
	else if (selected_chessmen == nullptr) {
		try {
			chessmen* clicked_chessmen = findChessmen(field);
			if (clicked_chessmen->getPlayer() != player) {
				return cg::enemy;
			}
			else {
				selected_chessmen = clicked_chessmen;
				return cg::selected;
			}
		}
		catch (const std::exception& exception) {
			return cg::emptyfield;
		}
	}
	else {
		move changes;
		changes.current_player = current_player;
		const auto result = moveCharacter(field, &changes, cg::oncetheoretical);
		if (result != cg::sucess) {
			if (result == cg::wouldbecheck) {
				return cg::checked;
			}
			else if (result == cg::impossible) {
				return cg::impmove;
			}
		}
		else {
			selected_chessmen->setHasMoved(TRUE);
			//replacing pawn at the end of field with queen
			if (selected_chessmen->figure() == cg::pawn) {
				if (selected_chessmen->getPos().y == chessmen::fieldsize_y_start && selected_chessmen->getPlayer() == cg::black || selected_chessmen->getPos().y == chessmen::fieldsize_y_end && selected_chessmen->getPlayer() == cg::white) {
					cg::position pos = selected_chessmen->getPos();
					const cg::color col = selected_chessmen->getPlayer();
					movetoside(pos, &chessmen_onfield, &chessmen_onside, &changes, TRUE);
					createChessmen(getField(), cg::queen, pos, col, TRUE);
				}
			}
			selected_chessmen = nullptr;
			forwardmovetrace.clear();
			backwardmovetrace.push_back(std::make_unique<move>(changes));
			if (king_situation(cg::black) == cg::stale) {
				last_game_status = cg::bkstale;
				return cg::bkstale;
			}
			else if (king_situation(cg::white) == cg::stale) {
				last_game_status = cg::wkstale;
				return cg::wkstale;
			}
			else if (king_situation(cg::white) == cg::checkmate) {
				last_game_status = cg::wkmate;
				return cg::wkmate;
			}
			else if (king_situation(cg::black) == cg::checkmate) {
				last_game_status = cg::bkmate;
				return cg::bkmate;
			}
			else {
				return cg::next;
			}
		}
	}
	return cg::error;
}
