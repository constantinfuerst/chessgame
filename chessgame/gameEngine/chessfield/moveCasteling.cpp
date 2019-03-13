#include "pch.h"
#include "chessfield.h"

chessfield::move_sucess chessfield::moveCasteling(chessmen::position& selectedMove, move* movecounter, theoretical theoretical) {
	if (selected_chessmen->figure() == chessmen::king) {
		for (size_t i = 0; i < casteling(selected_chessmen->player_color).size(); i++) {
			auto newkingpos = std::get<0>(casteling(selected_chessmen->player_color)[i]);
			if (selectedMove.x == newkingpos.x && selectedMove.y == newkingpos.y) {
				auto oldrookpos = std::get<0>(std::get<1>(casteling(selected_chessmen->player_color)[i]));
				auto newrookpos = std::get<1>(std::get<1>(casteling(selected_chessmen->player_color)[i]));
				//check if the resulting positions are valid
				if (!chessmen::validpos(newrookpos) || !chessmen::validpos(newkingpos)) {
					break;
				}
				//check if newkingpos is empty, if it finds a character there break
				try {
					findChessmen(newkingpos);
					break;
				}
				catch (const std::exception& exception) {}
				//check if newrookpos is empty, if it finds a character there break
				try {
					findChessmen(newrookpos);
					break;
				}
				catch (const std::exception& exception) {}
				//now try to do the move, if the rook at oldrookpos isn't found break
				try {
					if (findChessmen(oldrookpos)->figure() == chessmen::rook) {
						if (theoretical == onlytheoretical) {
							return sucess;
						}
						else {
							movetoempty(oldrookpos, newrookpos, &chessmen_onfield, movecounter);
							movetoempty(selected_chessmen->board_position, newkingpos, &chessmen_onfield, movecounter);
							return sucess;
						}
					}
				}
				catch (const std::exception& exception) {
					break;
				}
			}
		}
	}
	return impossible;
}

chessfield::castelingvec chessfield::casteling(chessmen::color& player) {
	castelingvec returnvec;
	chessmen* king = nullptr;
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->figure() == chessmen::king && chessmen_onfield[i]->player_color == player && chessmen_onfield[i]->hasMoved == FALSE) {
			king = chessmen_onfield[i].get();
			for (size_t j = 0; j < chessmen_onfield.size(); j++) {
				if (chessmen_onfield[j]->figure() == chessmen::rook && chessmen_onfield[j]->player_color == player && chessmen_onfield[j]->hasMoved == FALSE) {
					if (chessmen_onfield[j]->board_position.y == king->board_position.y) {
						chessboard virtualfield = copyChessboard(&chessmen_onfield);
						const unsigned int xposrook = chessmen_onfield[j]->board_position.x;
						unsigned int xposking = king->board_position.x;
						int kingmove = 0;
						int rookmove = 0;
						while (TRUE) {
							if (xposking < xposrook) {
								xposking++;
								virtualfield[i]->board_position.x = xposking;
								kingmove = +2; rookmove = +1;
							}
							else if (xposking > xposrook) {
								xposking--;
								virtualfield[i]->board_position.x = xposking;
								kingmove = -2;  rookmove = -1;
							}
							else if (xposking == xposrook) {
								//found a possible casteling move
								chessmen::position newkingpos = { king->board_position.x + kingmove, king->board_position.y };
								chessmen::position oldrookpos = chessmen_onfield[j]->board_position;
								chessmen::position newrookpos = { chessmen_onfield[i]->board_position.x + rookmove, chessmen_onfield[j]->board_position.y };
								returnvec.push_back(castelingtpl(newkingpos, posswaptpl(oldrookpos, newrookpos)));
								break;
							}
							else {
								break;
							}
							//check for empty field and the king not being check there
							if (check_check(player, &virtualfield) == check) {
								break;
							}
							else {
								try {
									findChessmen({ xposking, king->board_position.y }, &virtualfield);
								}
								catch (const std::exception& exception) {
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return returnvec;
}
