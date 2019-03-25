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
#include "chessmen.h"

//function definitions for the pawn

chessmen* pawn::clone() const {
	return new pawn(*this);
}

cg::chessfigure pawn::figure() {
	return cg::pawn;
}

//returns the legal moves of a pawn on a specific board
std::vector<cg::position> pawn::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	cg::position hitleft;
	cg::position hitright;
	cg::position onestep;
	cg::position twostep;

	//set the moves of a pawn depending on which color it is
	if (player_color == cg::black) {
		hitleft.x = board_position.x - 1;
		hitleft.y = board_position.y - 1;
		hitright.x = board_position.x + 1;
		hitright.y = board_position.y - 1;
		onestep.x = board_position.x;
		onestep.y = board_position.y - 1;
		twostep.x = board_position.x;
		twostep.y = board_position.y - 2;
	}
	else {
		hitleft.x = board_position.x - 1;
		hitleft.y = board_position.y + 1;
		hitright.x = board_position.x + 1;
		hitright.y = board_position.y + 1;
		onestep.x = board_position.x;
		onestep.y = board_position.y + 1;
		twostep.x = board_position.x;
		twostep.y = board_position.y + 2;
	}

	if (board_position.y <= cg::fieldsize_y_end && board_position.y >= cg::fieldsize_y_start) {
		//if the pawn hasnt moved yet and the fields are emopty it can move to fields forward
		if (hasMoved == FALSE) {
			if (positiocheck(chessmen, onestep, player_color) == cg::empty_pos && positiocheck(
				chessmen, twostep, player_color) == cg::empty_pos && validpos(twostep)) {
				returnpos.push_back(twostep);
			}
		}
		//the pawn can move one field forward if that is empty
		if (positiocheck(chessmen, onestep, player_color) == cg::empty_pos && validpos(onestep)) {
			returnpos.push_back(onestep);
		}
		//if the pawn has an enemy to its left (diagonally) it can hit there
		if (positiocheck(chessmen, hitleft, player_color) == cg::enemy_pos && validpos(hitleft)) {
			returnpos.push_back(hitleft);
		}
		//if the pawn has an enemy to its right (diagonally) it can hit there
		if (positiocheck(chessmen, hitright, player_color) == cg::enemy_pos && validpos(hitright)) {
			returnpos.push_back(hitright);
		}
	}

	return returnpos;
}
