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

//check for the king being check in a specific chessboard from a specific player
cg::king_status chessfield::check_check(cg::color& player, chessboard* chessmen) {
	//get the current position of the king
	cg::position* kingpos = nullptr;
	for (auto& i : *chessmen) {
		if (i->getPlayer() == player) {
			if (i->figure() == cg::king) {
				kingpos = new cg::position(i->getPos());
				break;
			}
		}
	}
	//if the king was found
	if (kingpos != nullptr) {
		//for every chessmen on the field
		for (size_t i = 0; i < chessmen->size(); i++) {
			if (chessmen->at(i)->getPlayer() != player) {
				std::vector<cg::position> possible_moves = truePossibleMoves(chessmen->at(i).get(), chessmen, TRUE);
				//for every possible move of that chessmen
				for (auto& possible_move : possible_moves) {
					//if the chessmen could move to the kings position return check
					if (possible_move.x == kingpos->x && possible_move.y == kingpos->y) {
						delete kingpos;
						return cg::check;
					}
				}
			}
		}
	}
	//if no move to the kings position was found return not check
	delete kingpos;
	return cg::not_check;
}

//get the current status of the king depending on which player was entered
cg::king_status chessfield::king_situation(cg::color player) {
	//for every chessmen on the board
	for (auto& i : chessmen_onfield) {
		//if it is a friendly
		if (i->getPlayer() == player) {
			chessmen* current_chessmen = i.get();
			//for every possible move of this chessmen
			for (size_t j = 0; j < truePossibleMoves(current_chessmen, &chessmen_onfield).size(); j++) {
				//use a virtual version of the current chessboard
				chessboard theoretical_field = copyChessboard(&chessmen_onfield);
				chessboard theoretical_side;
				//execute the currently selected move on the virtual field
				cg::position selectedMove = truePossibleMoves(current_chessmen, &chessmen_onfield)[j];
				try {
					if (findChessmen(selectedMove)->getPlayer() != player) {
						movetoside(selectedMove, &theoretical_field, &theoretical_side, nullptr);
					}
					else {
						continue;
					}
				}
				catch (const std::exception& exception) {
				}
				auto pos = current_chessmen->getPos();
				movetoempty(pos, selectedMove, &theoretical_field, nullptr);
				//if the king is not in check with this theoretical board position, it is not checkmate
				if (check_check(player, &theoretical_field) == cg::not_check) {
					//return check if the king is check in the actual board position, duh
					if (check_check(player, &chessmen_onfield) == cg::check) {
						return cg::check;
					}
					//if not return none
					return cg::not_check;
				}
			}
		}
	}
	//if no check-free found the king is mate
	//if the king is currently not in check then its a stale
	if (check_check(player, &chessmen_onfield) == cg::not_check) {
		return cg::stale;
	}
	//else its a checkmate
	return cg::checkmate;
}
