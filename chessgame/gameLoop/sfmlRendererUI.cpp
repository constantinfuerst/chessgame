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
#include "sfmlRenderer.h"
#include "settings.h"

#ifdef WIN_DESKTOP_GUI
#define cout(str)
#else
#include <iostream>
#define cout(str) std::cout << str << std::endl;
#endif

//closes a child window and its contents if it finds one
void sfmlRenderer::closeChild() const {
	//try to find a tgui object called "child_window" and if found close it
	auto child = gui->get("child_window");
	if (child != nullptr) {
		cout("child window closed");
		displayingUI = cg::nodisplay;
		redraw = TRUE;
		gui->remove(child);
		child->setVisible(FALSE);
		child->disconnectAll();
	}
}

//create a child tgui window for a specific content type
void sfmlRenderer::ui_child_create(cg::ui_type type, std::string message) const {
	//if we are allready displaying UI prevent overlap by returning
	if (displayingUI != cg::nodisplay) {
		return;
	}

	unsigned int width;
	unsigned int height;
	//set the dimensions required for the intended content
	if (type == cg::ui_message && !message.empty()) {
		width = 200;
		height = 100;
	}
	else if (type == cg::ui_savegame) {
		width = 200;
		height = 150;
	}
	else if (type == cg::ui_loadgame) {
		width = 300;
		height = 150;
	}
	else if (type == cg::ui_newgame) {
		width = 200;
		height = 100;
	}
	else {
		return;
	}

	//now create the child window and fill the gui_window struct to pass on to the ui content functions
	auto child = tgui::ChildWindow::create();
	cg::gui_window ui_window = {
		tgui::Theme{ ASSETS_DIR + "guirenderer\\TransparentGrey.txt" },
		child,
		width,
		height,
		30
	};

	static const unsigned int posx = (screen_dims.width / 2) - (ui_window.width / 2);
	static const unsigned int posy = ((screen_dims.height / 2) - 50) - (ui_window.width / 2);

	cout("constructing child window");
	//construct the child window
	child->setVisible(TRUE);
	child->setResizable(FALSE);
	child->setPositionLocked(TRUE);
	child->setRenderer(ui_window.theme.getRenderer("ChildWindow"));
	child->setSize(ui_window.width, ui_window.height);
	child->setPosition(posx, posy);
	child->connect("closed", [&]() {
		this->closeChild();
	});
	gui->add(child, "child_window");

	//now execute the function matching the selected window content type
	if (type == cg::ui_message && !message.empty()) {
		redraw = TRUE;
		displayingUI = cg::display_noupdate;
		ui_child_addmessage(ui_window, message);
	}
	else if (type == cg::ui_savegame) {
		redraw = TRUE;
		displayingUI = cg::display_update;
		ui_child_addcreatesavegame(ui_window);
	}
	else if (type == cg::ui_loadgame) {
		redraw = TRUE;
		displayingUI = cg::display_update;
		ui_child_addloadsavegame(ui_window, 0);
	}
	else if (type == cg::ui_newgame) {
		redraw = TRUE;
		displayingUI = cg::display_noupdate;
		ui_child_addnewgame(ui_window, message);
	}
	else {
		this->closeChild();
	}
}

//function used to populate a tgui child window
//displays a message and close button
void sfmlRenderer::ui_child_addmessage(cg::gui_window child_window, const std::string& message) const {
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = child_window.height / 4;

	//create label
	label->setRenderer(child_window.theme.getRenderer("Label"));
	label->setText(message);
	label->setPosition(child_window.padding, child_window.padding / 2);
	label->setTextSize(child_window.padding / 2);
	label->setAutoSize(TRUE);
	child_window.child_window->add(label);
	//create close/accept button
	yesbutton->setRenderer(child_window.theme.getRenderer("Button"));
	yesbutton->setPosition((child_window.width - buttonwidth) / 2, child_window.height - buttonheight);
	yesbutton->setText("ok");
	yesbutton->setSize(buttonwidth, buttonheight);
	yesbutton->connect("pressed", [&]() {
		cout("close message");
		this->closeChild();
	});
	child_window.child_window->add(yesbutton);
}

//function used to populate a tgui child window
//displays the files and loads a gamestate ".csg" file
//index is used to update the files displayed
void sfmlRenderer::ui_child_addloadsavegame(cg::gui_window child_window, int index) const {
	auto upbutton = tgui::Button::create();
	auto downbutton = tgui::Button::create();
	static bool constructed = FALSE;

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = (child_window.height - 20) / 5;

	std::vector<std::string> files = getFiles();

	//if the buttons arent yet constructed do so
	if (constructed == FALSE) {
		//add button for going up through the files
		upbutton->setRenderer(child_window.theme.getRenderer("Button"));
		upbutton->setPosition(child_window.width - (2 * child_window.padding), child_window.padding);
		upbutton->setText("up");
		upbutton->setSize(2 * child_window.padding, child_window.padding / 2);
		upbutton->connect("pressed", [=]() {
			cout("up pressed");
			if (index - 5 >= 0) {
				cout("5 up")
				this->ui_child_addloadsavegame(child_window, index - 5);
			}
		});
		//add button for going down through the files
		child_window.child_window->add(upbutton);
		downbutton->setRenderer(child_window.theme.getRenderer("Button"));
		downbutton->setPosition(child_window.width - (2 * child_window.padding), child_window.height - child_window.padding);
		downbutton->setText("down");
		downbutton->setSize(2 * child_window.padding, child_window.padding / 2);
		downbutton->connect("pressed", [=]() {
			cout("down pressed");
			const int maxindex = 10 * ((files.size() + 9) / 10);
			if (index + 5 < maxindex) {
				cout("5 down")
				this->ui_child_addloadsavegame(child_window, index + 5);
			}
		});
		child_window.child_window->add(downbutton);
	}

	//for every file
	for (size_t i = index; i < files.size(); i++) {
		//create a clickable button
		auto button = tgui::Button::create();
		unsigned int btny = ((i - index) * buttonheight) + 10;
		std::string gamename;
		{
			//read the name of the savegame from the json data
			nlohmann::json json;
			boost::filesystem::fstream json_input;
			json_input.open(files[i]);
			if (!json_input.is_open())
				break;
			json_input >> json;
			gamename = json["name"].get<std::string>();
			json_input.close();
		}
		button->setSize(buttonwidth, buttonheight);
		button->setPosition(5, btny);
		//display the name and not filename of that game
		button->setText(gamename);
		button->setTextSize(8);
		std::string filename = files.at(i);
		button->connect("pressed", [=]() {
			cout("loadgame");
			this->loadSavegame(filename);
			this->closeChild();
		});
		child_window.child_window->add(button);
		//when we reached the 5th gamefile break
		if (i - index == 4)
			break;
	}
}

//function used to populate a tgui child window
//displays user input for a file, enables saving to a file and closing
void sfmlRenderer::ui_child_addcreatesavegame(cg::gui_window child_window) const {
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto filename = tgui::EditBox::create();

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = child_window.height / 4;

	//create the window description
	label->setRenderer(child_window.theme.getRenderer("Label"));
	label->setText("Please enter a name\nfor your savegame");
	label->setPosition(child_window.padding, child_window.padding / 2);
	label->setTextSize(child_window.padding / 2);
	child_window.child_window->add(label);
	//create the filename input
	filename->setSize({ child_window.width - (child_window.padding * 2), child_window.padding });
	filename->setPosition({ child_window.padding, child_window.padding * 2 });
	filename->setDefaultText("Filename");
	child_window.child_window->add(filename);
	//create the accept/save button
	yesbutton->setRenderer(child_window.theme.getRenderer("Button"));
	yesbutton->setPosition((child_window.width - buttonwidth) / 2, child_window.height - buttonheight);
	yesbutton->setText("save");
	yesbutton->setSize(buttonwidth, buttonheight);
	yesbutton->connect("pressed", [=]() {
		cout("savegame");
		this->createSavegame(filename);
		this->closeChild();
	});
	child_window.child_window->add(yesbutton);
}

//function used to populate a tgui child window
//asks the user to confirm whether to restart the game with a changable message
void sfmlRenderer::ui_child_addnewgame(cg::gui_window child_window, std::string message) const {
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto nobutton = tgui::Button::create();

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = child_window.height / 4;

	//if no message was entered use the standard one
	if (message.empty())
		message = "Do you really want\nto start a new game?";

	//create the label with the specified message
	label->setRenderer(child_window.theme.getRenderer("Label"));
	label->setText(message);
	label->setPosition(child_window.padding, child_window.padding / 2);
	label->setTextSize(child_window.padding / 2);
	child_window.child_window->add(label);
	//create a yes/reload button
	yesbutton->setRenderer(child_window.theme.getRenderer("Button"));
	yesbutton->setPosition(0, child_window.height - buttonheight);
	yesbutton->setText("yes");
	yesbutton->setSize(buttonwidth, buttonheight);
	yesbutton->connect("pressed", [&]() {
		cout("yes newgame");
		game_status = cg::restart;
		this->closeChild();
	});
	child_window.child_window->add(yesbutton);
	//create a no/dont reload button
	nobutton->setRenderer(child_window.theme.getRenderer("Button"));
	nobutton->setPosition(buttonwidth, child_window.height - buttonheight);
	nobutton->setText("no");
	nobutton->setSize(buttonwidth, buttonheight);
	nobutton->connect("pressed", [&]() {
		cout("no newgame");
		this->closeChild();
	});
	child_window.child_window->add(nobutton);
}