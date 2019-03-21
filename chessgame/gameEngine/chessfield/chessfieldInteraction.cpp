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

//used to combine any "special moves" (displays casteling if possible)
std::vector<cg::position> chessfield::truePossibleMoves(chessmen* chessmen, chessboard* chessboard, bool dontCheckMate) {
	//create the returnvector and fill with all possible moves for the chessmen
	std::vector<cg::position> returnpos = chessmen->possibleMoves(chessboard);
	bool clear = FALSE;
	if (dontCheckMate != TRUE) {
		//if no selected chessmen, select the entered one
		if (selected_chessmen == nullptr) {
			clear = TRUE;
			selected_chessmen = chessmen;
		}

		//erase the impossible moves from the vector
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

		if (selected_chessmen->figure() == cg::king) {
			auto casteling_pos = casteling(selected_chessmen->getPlayer());
			for (size_t i = 0; i < casteling_pos.size(); i++) {
				returnpos.push_back(casteling_pos[i].newkpos);
			}
		}

		//if we previously added a new chessmen to selected unselect
		if (clear == TRUE) {
			selected_chessmen = nullptr;
		}
	}
	return returnpos;
}

//the main function that handles click input by the user with the GUI being a proxy
cg::full_game_status chessfield::clickfield(cg::position field, cg::color player) {
	if (last_game_status >= 8) { //when the game is allready over return the last state
		return last_game_status;
	}
	//if no chessmen is selected try to select one on the clicked position
	else if (selected_chessmen == nullptr) {
		//try to find a chessmen on the clicked position and if found select it
		try {
			chessmen* clicked_chessmen = findChessmen(field);
			if (clicked_chessmen->getPlayer() != player) {
				//an enemy was clicked
				return cg::enemy;
			}
			else {
				selected_chessmen = clicked_chessmen;
				//a valid chessmen was selected
				return cg::selected;
			}
		}
		catch (const std::exception& exception) {
			//an empty field was clicked
			return cg::emptyfield;
		}
	}
	//if a chessmen is allready selected
	else {
		//prepare the move to be put on traceback
		move changes;
		changes.current_player = current_player;
		//make the move with the selected position and handle output with "result"
		const auto result = moveCharacter(field, &changes, cg::oncetheoretical);
		//if the move didnt successfully complete check what happened
		if (result != cg::sucess) {
			//if the move would result in check return "check"
			if (result == cg::wouldbecheck) {
				return cg::checked;
			}
			//if the move is impossible return "impmove"
			else if (result == cg::impossible) {
				return cg::impmove;
			}
		}
		//if the move completed successfully
		else {
			//set the "hasmoved" state of the selected chessmen to TRUE
			selected_chessmen->setHasMoved(TRUE);
			//replacing pawn at the end of field with queen
			if (selected_chessmen->figure() == cg::pawn) {
				//if the pawn is at the end of the field respectively
				if (selected_chessmen->getPos().y == cg::fieldsize_y_start && selected_chessmen->getPlayer() == cg::black || selected_chessmen->getPos().y == cg::fieldsize_y_end && selected_chessmen->getPlayer() == cg::white) {
					//do all the replacing
					cg::position pos = selected_chessmen->getPos();
					const cg::color col = selected_chessmen->getPlayer();
					movetoside(pos, &chessmen_onfield, &chessmen_onside, &changes);
					createChessmen(getField(), cg::queen, pos, col, TRUE);
				}
			}
			//unselect the chessmen now and clear the forwardmovetrace (as it is inaccessible now)
			selected_chessmen = nullptr;
			forwardmovetrace.clear();
			backwardmovetrace.push_back(std::make_unique<move>(changes));
			//check the state of the king now
			if (king_situation(cg::black) == cg::stale) {
				//if the black king is stale return so
				last_game_status = cg::bkstale;
				return cg::bkstale;
			}
			else if (king_situation(cg::white) == cg::stale) {
				//if the white king is stale return so
				last_game_status = cg::wkstale;
				return cg::wkstale;
			}
			else if (king_situation(cg::white) == cg::checkmate) {
				//if the white king is mate return so
				last_game_status = cg::wkmate;
				return cg::wkmate;
			}
			else if (king_situation(cg::black) == cg::checkmate) {
				//if the black king is mate return so
				last_game_status = cg::bkmate;
				return cg::bkmate;
			}
			else {
				//if no game-end is reached return next
				return cg::next;
			}
		}
	}
	//if control flow end was reached without returning a valid state return an error
	return cg::error;
}
