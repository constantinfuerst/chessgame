/*
PROGRAMM METADATA:
	name:           chessgame
	description:    chessgame project I made to learn OOP, using sfml combined with tgui or a console as output
	copyright:      (c) 2019 Constantin Fürst constantin@fuersten.info
LICENSE:
   This work is created by Constantin Fürst.
   If you are unsure if your use of this project is permitted or you'd like to use it in a way prohibited by the following license, please message "constantin@fuersten.info".
   This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
   To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-nd/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
DISCLAIMER:
THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS 'AS IS' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
 */

#include "pch.h"
#include "chessfield.h"

cg::move_sucess chessfield::moveCasteling(cg::position& selectedMove, move* movecounter, const cg::theoretical theoretical) {
	//grab the possible castling moves and check if one of them matches the selected move
	auto casteling_pos = casteling(selected_chessmen->getPlayer());
	for (size_t i = 0; i < casteling_pos.size(); i++) {
		//if the selected move matches the castling move
		if (selectedMove.x == casteling_pos[i].newkpos.x && selectedMove.y == casteling_pos[i].newkpos.y) {
			//try the move, if the rook at oldrookpos isn't found break
			try {
				//grab the rook from its "old" (or current) position
				if (findChessmen(casteling_pos[i].oldrpos)->figure() == cg::rook) {
					if (theoretical == cg::onlytheoretical) {
						return cg::sucess;
					}
					//set the new position
					auto pos = selected_chessmen->getPos();
					movetoempty(casteling_pos[i].oldrpos, casteling_pos[i].newrpos, &chessmen_onfield, movecounter);
					movetoempty(pos, casteling_pos[i].newkpos, &chessmen_onfield, movecounter);
					return cg::sucess;
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

	//go through the field and find the king of the entered player
	for (size_t i = 0; i < chessmen_onfield.size(); i++) {
		if (chessmen_onfield[i]->figure() == cg::king && chessmen_onfield[i]->getPlayer() == player && chessmen_onfield[i]->getHasMoved() == FALSE) {
			chessmen* king = chessmen_onfield[i].get();
			//now find any rook of the entered player that hasn't been moved 
			for (auto& j : chessmen_onfield) {
				if (j->figure() == cg::rook && j->getPlayer() == player &&
					j->getHasMoved() == FALSE) {
					//when the king and the rook are on the same vertical position continue
					if (j->getPos().y == king->getPos().y) {
						//create a virtual field
						chessboard virtualfield = copyChessboard(&chessmen_onfield);
						const unsigned int xposrook = j->getPos().x;
						unsigned int xposking = king->getPos().x;
						int kingmove = 0;
						int rookmove = 0;
						//repeat this until we have an invalid position or a match
						while (TRUE) {
							//when the king is on the left hand side of the rook
							if (xposking < xposrook) {
								//move to king one step to the right
								xposking++;
								const cg::position newkingpos = {xposking, selected_chessmen->getPos().y};
								//if the new position isn't empty we cant move there -> break
								if (chessmen::positiocheck(&virtualfield, newkingpos, selected_chessmen->getPlayer()) != cg::empty_pos &&
									findChessmen(newkingpos, &virtualfield)->figure() != cg::rook)
									break;
								//now update the kings position (on the virtual field)
								virtualfield[i]->setPos().x = xposking;
								kingmove = +2;
								rookmove = +1;
								//is the king check with this position? if so we cant move there
								if (check_check(player, &virtualfield) == cg::check)
									break;
							}
								//when the king is on the right hand side of the rook
							else if (xposking > xposrook) {
								//move to king one step to the left
								xposking--;
								const cg::position newkingpos = {xposking, selected_chessmen->getPos().y};
								//if the new position isn't empty we cant move there -> break
								if (chessmen::positiocheck(&virtualfield, newkingpos, selected_chessmen->getPlayer()) != cg::empty_pos &&
									findChessmen(newkingpos, &virtualfield)->figure() != cg::rook)
									break;
								//now update the kings position (on the virtual field)
								virtualfield[i]->setPos().x = xposking;
								kingmove = -2;
								rookmove = -1;
								//is the king check with this position? if so we cant move there
								if (check_check(player, &virtualfield) == cg::check)
									break;
							}
								//when the king has reached the rook it must be a valid castling move
							else if (xposking == xposrook) {
								//calculate the new positions for king and rook and push these to the return vector
								const cg::position oldrookpos = j->getPos();
								const cg::position newrookpos = {
									chessmen_onfield[i]->getPos().x + rookmove, j->getPos().y
								};
								const cg::position newkingpos = {king->getPos().x + kingmove, king->getPos().y};

								//found a possible castling move
								casteling_str move{
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
