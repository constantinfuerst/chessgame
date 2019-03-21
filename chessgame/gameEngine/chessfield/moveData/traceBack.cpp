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

//removes a chessmen on a specified position from the normal field
void move::removeChessmen(chessboard& onfield, const cg::position pos) {
	for (size_t i = 0; i < onfield.size(); i++) {
		if (onfield.at(i)->getPos().x == pos.x && onfield.at(i)->getPos().y == pos.y) {
			onfield.erase(onfield.begin() + i);
			break;
		}
	}
}

//removes a specified chesspiece that was last positioned on a specified position from the side
void move::removeChessmen(chessboard& onside, const cg::position pos, cg::chessfigure& figure) {
	for (size_t i = 0; i < onside.size(); i++) {
		if (onside.at(i)->getPos().x == pos.x && onside.at(i)->getPos().y == pos.y && onside.at(i)->figure() == figure) {
			onside.erase(onside.begin() + i);
			break;
		}
	}
}

//moves a chessmen back
void move::moveBack(chessboard& onfield, const cg::position oldpos, const cg::position currentpos, const bool hasmovedold) {
	for (auto& i : onfield) {
		if (i->getPos().x == currentpos.x && i->getPos().y == currentpos.y) {
			i->setPos(oldpos);
			i->setHasMoved(hasmovedold);
		}
	}
}