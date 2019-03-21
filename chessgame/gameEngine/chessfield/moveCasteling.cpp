#include "pch.h"
#include "chessfield.h"

cg::move_sucess chessfield::moveCasteling(cg::position& selectedMove, move* movecounter, cg::theoretical theoretical) {
	//grab the possible casteling moves and check if one of them matches the selected move
	auto casteling_pos = casteling(selected_chessmen->getPlayer());
	for (size_t i = 0; i < casteling_pos.size(); i++) {
		//if the selected move matches the casteling move
		if (selectedMove.x == casteling_pos[i].newkpos.x && selectedMove.y == casteling_pos[i].newkpos.y) {
			//try the move, if the rook at oldrookpos isn't found break
			try {
				//grab the rook from its "old" (or current) position
				if (findChessmen(casteling_pos[i].oldrpos)->figure() == cg::rook) {
					if (theoretical == cg::onlytheoretical) {
						return cg::sucess;
					}
					else {
						//set the new position
						auto pos = selected_chessmen->getPos();
						movetoempty(casteling_pos[i].oldrpos, casteling_pos[i].newrpos, &chessmen_onfield, movecounter);
						movetoempty(pos, casteling_pos[i].newkpos, &chessmen_onfield, movecounter);
						return cg::sucess;
					}
				}
			}
			catch (const std::exception& exception) {
				break;
			}
		}
	}
	return cg::impossible;
}

chessfield::castelingvec chessfield::casteling(cg::color player) {
	castelingvec returnvec;
	//if the selected chessmen is not a king or has moved return empty
	if (selected_chessmen->figure() != cg::king || selected_chessmen->getHasMoved() == TRUE)
		return returnvec;

	chessmen* king = nullptr;
	//go through the field and find the king of the entered player
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->figure() == cg::king && chessmen_onfield[i]->getPlayer() == player && chessmen_onfield[i]->getHasMoved() == FALSE) {
			king = chessmen_onfield[i].get();
			//now find any rook of the entered player that hasn't been moved 
			for (size_t j = 0; j < chessmen_onfield.size(); j++) {
				if (chessmen_onfield[j]->figure() == cg::rook && chessmen_onfield[j]->getPlayer() == player && chessmen_onfield[j]->getHasMoved() == FALSE) {
					//when the king and the rook are on the same vertical position continue
					if (chessmen_onfield[j]->getPos().y == king->getPos().y) {
						//create a virtual field
						chessboard virtualfield = copyChessboard(&chessmen_onfield);
						const unsigned int xposrook = chessmen_onfield[j]->getPos().x;
						unsigned int xposking = king->getPos().x;
						int kingmove = 0;
						int rookmove = 0;
						//repeat this until we have an invalid position or a match
						while (TRUE) {
							//when the king is on the left hand side of the rook
							if (xposking < xposrook) {
								//move to king one step to the right
								xposking++;
								const cg::position newkingpos = { xposking, selected_chessmen->getPos().y };
								//if the new position isnt empty we cant move there -> break
								if (chessmen::positiocheck(&virtualfield, newkingpos, selected_chessmen->getPlayer()) != cg::empty_pos && findChessmen(newkingpos, &virtualfield)->figure() != cg::rook)
									break;
								//now update the kings position (on the vitual field)
								virtualfield[i]->setPos().x = xposking;
								kingmove = +2; rookmove = +1;
								//is the king check with this position? if so we cant move there
								if (check_check(player, &virtualfield) == cg::check)
									break;
							}
							//when the king is on the right hand side of the rook
							else if (xposking > xposrook) {
								//move to king one step to the left
								xposking--;
								const cg::position newkingpos = { xposking, selected_chessmen->getPos().y };
								//if the new position isnt empty we cant move there -> break
								if (chessmen::positiocheck(&virtualfield, newkingpos, selected_chessmen->getPlayer()) != cg::empty_pos && findChessmen(newkingpos, &virtualfield)->figure() != cg::rook)
									break;
								//now update the kings position (on the vitual field)
								virtualfield[i]->setPos().x = xposking;
								kingmove = -2;  rookmove = -1;
								//is the king check with this position? if so we cant move there
								if (check_check(player, &virtualfield) == cg::check)
									break;
							}
							//when the king has reached the rook it must be a valid casteling move
							else if (xposking == xposrook) {
								//calculate the new positions for king and rook and push these to the return vector
								const cg::position oldrookpos = chessmen_onfield[j]->getPos();
								const cg::position newrookpos = { chessmen_onfield[i]->getPos().x + rookmove, chessmen_onfield[j]->getPos().y };
								const cg::position newkingpos = { king->getPos().x + kingmove, king->getPos().y };

								//found a possible casteling move
								casteling_str move {
									oldrookpos,
									newrookpos,
									newkingpos
								};
								returnvec.push_back(move);
								break;
							}
							else {
								break;
							}
						}
					}
				}
			}
		}
	}
	return returnvec;
}
