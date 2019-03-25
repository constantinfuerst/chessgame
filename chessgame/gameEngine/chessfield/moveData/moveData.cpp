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
#include "moveData.h"

//create a move and add it to the current changes counter
void move::makemove(const cg::position oldPosition, const cg::position newPosition, const cg::color player, const cg::chessfigure figure, const bool hasmovedold, const cg::moveType move) {
	chessmenMoved change;
	change.hasmovedold = hasmovedold;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = player;
	change.figure = figure;
	changes.push_back(change);
}

//create a move and add it to the current changes counter
void move::makemove(chessmen* chessmen, const cg::position oldPosition, const cg::position newPosition, const bool hasmovedold, const cg::moveType move) {
	chessmenMoved change;
	change.hasmovedold = hasmovedold;
	change.move = move;
	change.oldPosition = oldPosition;
	change.newPosition = newPosition;
	change.player = chessmen->getPlayer();
	change.figure = chessmen->figure();
	changes.push_back(change);
}

//push a move to the current changes counter
void move::pushmove(const chessmenMoved& movedata) {
	changes.push_back(movedata);
}

move::move(const move& mve) {
	for (size_t i = 0; i < mve.changes.size(); i++) {
		chessmenMoved changepush;
		changepush.figure = mve.changes[i].figure;
		changepush.hasmovedold = mve.changes[i].hasmovedold;
		changepush.player = mve.changes[i].player;
		changepush.oldPosition = mve.changes[i].oldPosition;
		changepush.move = mve.changes[i].move;
		changepush.newPosition = mve.changes[i].newPosition;
		changes.push_back(changepush);
	}
	current_player = mve.current_player;
}
