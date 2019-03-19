#include "pch.h"
#include "chessfield.h"

//TODO: merge these in one function

cg::move_sucess chessfield::moveCasteling(cg::position& selectedMove, move* movecounter, cg::theoretical theoretical) {
	if (selected_chessmen->figure() == cg::king) {
		auto player = selected_chessmen->getPlayer();
		for (size_t i = 0; i < casteling(player).size(); i++) {
			std::unique_ptr<cg::position> newkingpos = std::make_unique<cg::position>(casteling(player)[i].newkpos);
			std::unique_ptr<cg::position> newrookpos = std::make_unique<cg::position>(casteling(player)[i].newrpos);
			std::unique_ptr<cg::position> oldrookpos = std::make_unique<cg::position>(casteling(player)[i].oldrpos);
			if (selectedMove.x == newkingpos->x && selectedMove.y == newkingpos->y) {
				//check if the resulting positions are valid
				if (!chessmen::validpos(*newrookpos) || !chessmen::validpos(*newkingpos)) {
					break;
				}
				//check if newkingpos is empty, if it finds a character there break
				try {
					findChessmen(*newkingpos);
					break;
				}
				catch (const std::exception& exception) {}
				//check if newrookpos is empty, if it finds a character there break
				try {
					findChessmen(*newrookpos);
					break;
				}
				catch (const std::exception& exception) {}
				//now try to do the move, if the rook at oldrookpos isn't found break
				try {
					if (findChessmen(*oldrookpos)->figure() == cg::rook) {
						if (theoretical == cg::onlytheoretical) {
							return cg::sucess;
						}
						else {
							auto pos = selected_chessmen->getPos();
							movetoempty(*oldrookpos, *newrookpos, &chessmen_onfield, movecounter, FALSE);
							movetoempty(pos, *newkingpos, &chessmen_onfield, movecounter, FALSE);
							return cg::sucess;
						}
					}
				}
				catch (const std::exception& exception) {
					break;
				}
			}
		}
	}
	return cg::impossible;
}

chessfield::castelingvec chessfield::casteling(cg::color& player) {
	castelingvec returnvec;
	chessmen* king = nullptr;
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->figure() == cg::king && chessmen_onfield[i]->getPlayer() == player && chessmen_onfield[i]->getHasMoved() == FALSE) {
			king = chessmen_onfield[i].get();
			for (size_t j = 0; j < chessmen_onfield.size(); j++) {
				if (chessmen_onfield[j]->figure() == cg::rook && chessmen_onfield[j]->getPlayer() == player && chessmen_onfield[j]->getHasMoved() == FALSE) {
					if (chessmen_onfield[j]->getPos().y == king->getPos().y) {
						chessboard virtualfield = copyChessboard(&chessmen_onfield);
						const unsigned int xposrook = chessmen_onfield[j]->getPos().x;
						unsigned int xposking = king->getPos().x;
						int kingmove = 0;
						int rookmove = 0;
						while (TRUE) {
							if (xposking < xposrook) {
								xposking++;
								virtualfield[i]->setPos().x = xposking;
								kingmove = +2; rookmove = +1;
							}
							else if (xposking > xposrook) {
								xposking--;
								virtualfield[i]->setPos().x = xposking;
								kingmove = -2;  rookmove = -1;
							}
							else if (xposking == xposrook) {
								//found a possible casteling move
								casteling_str move {
									{ king->getPos().x + kingmove, king->getPos().y },
									chessmen_onfield[j]->getPos(),
									{ chessmen_onfield[i]->getPos().x + rookmove, chessmen_onfield[j]->getPos().y }
								};
								returnvec.push_back(move);
								break;
							}
							else {
								break;
							}
							//check for empty field and the king not being check there
							if (check_check(player, &virtualfield) == cg::check) {
								break;
							}
							else {
								try {
									findChessmen({ xposking, king->getPos().y }, &virtualfield);
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
