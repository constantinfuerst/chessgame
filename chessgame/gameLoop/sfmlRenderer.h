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

#include "gameEngine/chessfield/chessfield.h"
#include "gameEngine/cg_defs.h"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

class sfmlRenderAsstes final {
private:
	cg::spr_ptr_vec ui_elements;
	cg::idim board_dims = {0, 0};
	cg::idim true_board_dims = {0, 0};
	std::vector<cg::chessmen_sprite> sprites;

public:
	static sfmlRenderAsstes* get();
	void loadAssets();
	sf::Sprite* chessboard = nullptr;
	const cg::idim& getBoarddims() const;
	const cg::idim& getTrueBoarddims() const;
	cg::spr_ptr_vec& getUiElems();
	sf::Sprite* getSprite(cg::color player, cg::chessfigure figure) const;
};

class sfmlRenderer final {
private:
	//scaling the fields
	cg::idim field_dims;
	cg::idim screen_dims;
	//UI elements
	int ui_element_width;
	//efficiency
	mutable cg::uidisplay displayingUI;
	mutable bool redraw = TRUE;
	mutable bool paused = FALSE;
	//game status evaluator
	mutable cg::game_status game_status;
	//mouse button
	bool lmb_press = FALSE;

	static std::vector<std::string> getFiles();

	void createSavegame(tgui::EditBox::Ptr filename) const;
	void loadSavegame(const std::string& filename) const;

	void closeChild() const;
	void ui_child_create(cg::ui_type type, std::string message = "") const;
	void ui_child_addmessage(cg::gui_window child_window, const std::string& message) const;
	void ui_child_addloadsavegame(cg::gui_window child_window, int index) const;
	void ui_child_addcreatesavegame(cg::gui_window child_window) const;
	void ui_child_addnewgame(cg::gui_window child_window, std::string message) const;

	cg::position getclickedfield() const;
	int getclickedui() const;
	bool processUIInput(unsigned int ui_element) const;
	void processOutput(cg::full_game_status status) const;

	chessfield* game;
	tgui::Gui* gui;
	sf::RenderWindow* window;

	void render();
	void gameLogic();

public:
	int gameLoop();
	~sfmlRenderer();
	sfmlRenderer();
};
