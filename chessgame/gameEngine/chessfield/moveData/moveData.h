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

#pragma once

#include "pch.h"
#include "gameEngine/chessmen/chessmen.h"
#include "gameEngine/cg_defs.h"

class chessfield;

struct move {
	cg::color current_player = cg::white;
	typedef std::vector<std::unique_ptr<chessmen>> chessboard;

	struct chessmenMoved {
		bool hasmovedold;
		cg::chessfigure figure;
		cg::color player;
		cg::position oldPosition;
		cg::moveType move;
		cg::position newPosition;
	};

	std::vector<chessmenMoved> changes;
	void pushmove(const chessmenMoved& movedata);
	void makemove(cg::position oldPosition, cg::position newPosition, cg::color player, cg::chessfigure figure,
	              bool hasmovedold, cg::moveType move);
	void makemove(chessmen* chessmen, cg::position oldPosition, cg::position newPosition, bool hasmovedold,
	              cg::moveType move);
	static void removeChessmen(chessboard& onfield, cg::position pos);
	static void removeChessmen(chessboard& onside, cg::position pos, cg::chessfigure& figure);
	static void moveBack(chessboard& onfield, cg::position oldpos, cg::position currentpos, bool hasmovedold);
	move(const move& mve);
	move() = default;
};
