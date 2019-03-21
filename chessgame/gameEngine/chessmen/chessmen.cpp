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

//checks for a position to be valid (in the parameters of the currently used field)
bool chessmen::validpos(cg::position position) {
	if (position.x >= cg::fieldsize_x_start && position.x <= cg::fieldsize_x_end && position.y >= cg::fieldsize_y_start && position.y <= cg::fieldsize_y_end) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

//checks a position for its state (empty, enemy or friendly)
cg::position_status chessmen::positiocheck(chessboard* chessmen, cg::position pos, cg::color player) {
	//for every chessmen on the field
	for (auto& i : *chessmen) {
		if (i->board_position.x == pos.x && i->board_position.y == pos.y) {
			if (i->player_color == player) {
				//if there is a chessmen with the same position as "pos" that is of the same color as "player" it must be a used position by a friendly
				return cg::friendly_pos;
			}
			else {
				//if there is a chessmen with the same position as "pos" that is of a different color than "player" it must be a used position by an enemy
				return cg::enemy_pos;
			}
		}
	}
	//if no chessmen with the same position was found then it must be empty
	return cg::empty_pos;
}

chessmen::chessmen(cg::color color_input, cg::position position_input, bool moved) {
	board_position.x = position_input.x;
	board_position.y = position_input.y;
	player_color = color_input;
	hasMoved = moved;
}

chessmen::chessmen(cg::color color_input, unsigned int posx, unsigned int posy, bool moved){
	board_position.x = posx;
	board_position.y = posy;
	player_color = color_input;
	hasMoved = moved;
}

//getter for the position of a chessmen
cg::position chessmen::getPos() const {
	return this->board_position;
}

//getter for the player of a chessmen
cg::color chessmen::getPlayer() const {
	return this->player_color;
}

//getter for the hasMoved state of a chessmen
bool chessmen::getHasMoved() const {
	return this->hasMoved;
}

//setter for the hasMoved state of a chessmen
void chessmen::setHasMoved(bool hasMoved) {
	this->hasMoved = hasMoved;
}

//setter for the position of a chessmen taking in a new position
void chessmen::setPos(const cg::position pos) {
	if (validpos(pos)) {
		this->board_position = pos;
	}
}

//getter for the position of a chessmen returning a refference to manually set it
cg::position& chessmen::setPos() {
	return this->board_position;
}