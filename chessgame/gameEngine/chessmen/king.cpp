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

//function definitions for the king

chessmen* king::clone() const {
	return new king(*this);
}

cg::chessfigure king::figure() {
	return cg::king;
}

//returns the legal moves of a king on a specific board
std::vector<cg::position> king::possibleMoves(chessboard* chessmen) {
	std::vector<cg::position> returnpos;
	{
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (board_position.x + i <= cg::fieldsize_x_end && board_position.y + j <= cg::fieldsize_y_end) {
					const cg::position pos = {board_position.x + i, board_position.y + j};
					if (positiocheck(chessmen, pos, player_color) != cg::friendly_pos && validpos(pos)) {
						returnpos.push_back(pos);
					}
				}
			}
		}
	}
	return returnpos;
}
