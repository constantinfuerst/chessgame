#include "pch.h"
#include "sfmlRenderer.h"

#ifdef WIN_DESKTOP_GUI
#define cout(str)
#else
#include <iostream>
#define cout(str) std::cout << str << std::endl;
#endif

void sfmlRenderer::closeChild() const {

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

void sfmlRenderer::ui_child_create(cg::ui_type type, std::string message) const {
	unsigned int width;
	unsigned int height;

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

	auto child = tgui::ChildWindow::create();
	cg::gui_window ui_window = {
		tgui::Theme{ ASSETS_DIR + "guirenderer\\TransparentGrey.txt" },
		child,
		width,
		height,
		30
	};

	static const unsigned int posx = (screenWidth / 2) - (ui_window.width / 2);
	static const unsigned int posy = ((screenHeight / 2) - 50) - (ui_window.width / 2);

	if (displayingUI != cg::nodisplay) {
		return;
	}
	else {
		cout("constructing ui_savegame");

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
	}

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

void sfmlRenderer::ui_child_addmessage(cg::gui_window child_window, const std::string& message) const {
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static bool constructed = FALSE;
	static std::string old_message;

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = child_window.height / 4;

	if (message != old_message) {
		label->setText(message);
	}
	else if (constructed == FALSE) {
		label->setRenderer(child_window.theme.getRenderer("Label"));
		label->setText(old_message);
		label->setPosition(child_window.padding, child_window.padding  /2);
		label->setTextSize(child_window.padding / 2);
		label->setAutoSize(TRUE);
		child_window.child_window->add(label);
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
	else {
		cout("close message");
		this->closeChild();
	}

	old_message = message;
}

void sfmlRenderer::ui_child_addloadsavegame(cg::gui_window child_window, int index) const {
	auto upbutton = tgui::Button::create();
	auto downbutton = tgui::Button::create();
	static bool constructed = FALSE;

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = (child_window.height - 20) / 5;

	std::vector<std::string> files = getFiles();

	if (constructed == FALSE) {
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

	for (size_t i = index; i < files.size(); i++) {
		auto button = tgui::Button::create();
		unsigned int btny = ((i - index) * buttonheight) + 10;
		std::string gamename;
		{
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
		button->setText(gamename);
		button->setTextSize(8);
		std::string filename = files.at(i);
		button->connect("pressed", [=]() {
			cout("loadgame");
			this->loadSavegame(filename);
			this->closeChild();
		});
		child_window.child_window->add(button);
		if (i - index == 4)
			break;
	}
}

void sfmlRenderer::ui_child_addcreatesavegame(cg::gui_window child_window) const {
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto filename = tgui::EditBox::create();
	static bool constructed = FALSE;

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = child_window.height / 4;

	if (constructed == FALSE) {
		label->setRenderer(child_window.theme.getRenderer("Label"));
		label->setText("Please enter a name\nfor your savegame");
		label->setPosition(child_window.padding, child_window.padding / 2);
		label->setTextSize(child_window.padding / 2);
		child_window.child_window->add(label);
		filename->setSize({ child_window.width - (child_window.padding * 2), child_window.padding });
		filename->setPosition({ child_window.padding, child_window.padding * 2 });
		filename->setDefaultText("Filename");
		child_window.child_window->add(filename);
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
	else {
		cout("close savegame");
		this->closeChild();
	}
}

void sfmlRenderer::ui_child_addnewgame(cg::gui_window child_window, std::string message) const {
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto nobutton = tgui::Button::create();
	static bool constructed = FALSE;

	static const unsigned int buttonwidth = child_window.width / 2;
	static const unsigned int buttonheight = child_window.height / 4;

	if (message.empty())
		message = "Do you really want\nto start a new game?";

	if (constructed == FALSE) {
		label->setRenderer(child_window.theme.getRenderer("Label"));
		label->setText(message);
		label->setPosition(child_window.padding, child_window.padding / 2);
		label->setTextSize(child_window.padding / 2);
		child_window.child_window->add(label);
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
	else {
		cout("close savegame");
		this->closeChild();
	}
}