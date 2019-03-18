#include "pch.h"
#include "sfmlRenderer.h"

#ifdef WIN_DESKTOP_GUI
#define cout(str)
#else
#include <iostream>
#define cout(str) std::cout << str << std::endl;
#endif

//TODO: compact these functions, maybe create an asset singleton or manage shared or very different code in some other way, maybe create a UI class

void sfmlRenderer::ui_message(std::string message) const {
	static tgui::Theme theme{ ASSETS_DIR + "guirenderer\\TransparentGrey.txt" };
	static const unsigned int width = 200;
	static const unsigned int height = 75;
	static const unsigned int posx = (screenWidth / 2) - (width / 2);
	static const unsigned int posy = ((screenHeight / 2) - 50) - (height / 2);
	static const unsigned int buttonwidth = width / 2;
	static const unsigned int buttonheight = height / 4;
	static const unsigned int padding = 30;

	static bool constructed = FALSE;

	static auto child = tgui::ChildWindow::create();
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();

	if (displayingUI != nodisplay) {
		return;
	}
	else if (constructed == TRUE) {
		if (child->isVisible()) {
			cout("setting savegame visible FALSE");
			child->setVisible(FALSE);
			displayingUI = nodisplay;
		}
		else {
			label->setText(message);
			cout("setting savegame visible TRUE");
			child->setVisible(TRUE);
			displayingUI = display_noupdate;
		}
	}
	else {
		cout("constructing ui_savegame");
		constructed = TRUE;
		displayingUI = display_noupdate;

		child->setResizable(FALSE);
		child->setPositionLocked(TRUE);
		child->setRenderer(theme.getRenderer("ChildWindow"));
		child->setSize(width, height);
		child->setPosition(posx, posy);
		child->connect("closed", [&]() {
			cout("message closed");
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		gui->add(child);
		label->setRenderer(theme.getRenderer("Label"));
		label->setText(message);
		label->setPosition(padding, 10);
		label->setTextSize(padding / 2);
		label->setAutoSize(TRUE);
		child->add(label);
		yesbutton->setRenderer(theme.getRenderer("Button"));
		yesbutton->setPosition((width - buttonwidth) / 2, height - buttonheight);
		yesbutton->setText("ok");
		yesbutton->setSize(buttonwidth, buttonheight);
		yesbutton->connect("pressed", [&]() {
			cout("close message");
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		child->add(yesbutton);
	}
}

void sfmlRenderer::ui_loadgame(int index) {
	//TODO: display a "game name" stored in the .csg file

	static tgui::Theme theme{ ASSETS_DIR + "guirenderer\\TransparentGrey.txt" };
	static const unsigned int width = window->getSize().x - 20;
	static const unsigned int height = 200;
	static const unsigned int btnheight = (height - 20) / 5;
	static const unsigned int btnwidth = width - 75;
	static const unsigned int posx = (screenWidth / 2) - (width / 2);
	static const unsigned int posy = ((screenHeight / 2) - 50) - (height / 2);

	static auto child = tgui::ChildWindow::create();
	auto yesbutton = tgui::Button::create();
	auto nobutton = tgui::Button::create();

	std::vector<std::string> files = getFiles();

	if (displayingUI != nodisplay) {
		if (!child->isVisible()) {
			return;
		}
	}
	else {
		cout("constructing ui_savegame");
		displayingUI = display_update;

		child->setResizable(FALSE);
		child->setVisible(TRUE);
		child->setPositionLocked(TRUE);
		child->setRenderer(theme.getRenderer("ChildWindow"));
		child->setSize(width, height);
		child->setPosition(posx, posy);
		child->setTitle("Choose a savegame");
		child->connect("closed", [&]() {
			cout("ui_savegame closed");
			redraw = TRUE;
			displayingUI = nodisplay;
			child->setVisible(FALSE);
			child->removeAllWidgets();
			child->disconnectAll();
			child->destroy();
		});
		gui->add(child);
		yesbutton->setRenderer(theme.getRenderer("Button"));
		yesbutton->setPosition(btnwidth + 5, 10);
		yesbutton->setText("up");
		yesbutton->setSize(50, 15);
		yesbutton->connect("pressed", [=]() {
			cout("up pressed");
			if (index - 5 >= 0) {
				redraw = TRUE;
				displayingUI = nodisplay;
				child->setVisible(FALSE);
				child->removeAllWidgets();
				child->disconnectAll();
				child->destroy();
				cout("5 up")
					this->ui_loadgame(index - 5);
			}
		});
		child->add(yesbutton);
		nobutton->setRenderer(theme.getRenderer("Button"));
		nobutton->setPosition(btnwidth + 5, height - 25);
		nobutton->setText("down");
		nobutton->setSize(50, 15);
		nobutton->connect("pressed", [=]() {
			cout("down pressed");
			const int maxindex = 10 * ((files.size() + 9) / 10);
			if (index + 5 < maxindex) {
				redraw = TRUE;
				displayingUI = nodisplay;
				child->setVisible(FALSE);
				child->removeAllWidgets();
				child->disconnectAll();
				child->destroy();
				cout("5 down")
					this->ui_loadgame(index + 5);
			}
		});
		child->add(nobutton);

		for (size_t i = index; i < files.size(); i++) {
			auto button = tgui::Button::create();
			unsigned int btny = ((i - index) * btnheight) + 10;
			button->setSize(btnwidth, btnheight);
			button->setPosition(5, btny);
			button->setText(files[i]);
			button->setTextSize(8);
			std::string filename = files.at(i);
			button->connect("pressed", [=]() {
				cout("loadgame");
				displayingUI = nodisplay;
				redraw = TRUE;
				child->setVisible(FALSE);
				this->loadSavegame(filename);
			});
			child->add(button);
			if (i - index == 4)
				break;
		}
	}
}

void sfmlRenderer::ui_savegame() {
	//TODO: use the "game name" and store in the .csg file

	static tgui::Theme theme{ ASSETS_DIR + "guirenderer\\TransparentGrey.txt" };
	static const unsigned int width = 200;
	static const unsigned int height = 150;
	static const unsigned int posx = (screenWidth / 2) - (width / 2);
	static const unsigned int posy = ((screenHeight / 2) - 50) - (height / 2);
	static const unsigned int buttonwidth = width / 2;
	static const unsigned int buttonheight = height / 4;
	static const unsigned int padding = 30;

	static bool constructed = FALSE;

	static auto child = tgui::ChildWindow::create();
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto filename = tgui::EditBox::create();

	if (displayingUI != nodisplay) {
		return;
	}
	else if (constructed == TRUE) {
		if (child->isVisible()) {
			cout("setting savegame visible FALSE");
			child->setVisible(FALSE);
			displayingUI = nodisplay;
		}
		else {
			cout("setting savegame visible TRUE");
			child->setVisible(TRUE);
			displayingUI = display_update;
		}
	}
	else {
		cout("constructing ui_savegame");
		constructed = TRUE;
		displayingUI = display_update;

		child->setResizable(FALSE);
		child->setPositionLocked(TRUE);
		child->setRenderer(theme.getRenderer("ChildWindow"));
		child->setSize(width, height);
		child->setPosition(posx, posy);
		child->setTitle("Enter a filename");
		child->connect("closed", [&]() {
			cout("ui_savegame closed");
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		gui->add(child);
		label->setRenderer(theme.getRenderer("Label"));
		label->setText("Please enter a name\nfor your savegame");
		label->setPosition(padding, 10);
		label->setTextSize(padding / 2);
		child->add(label);
		filename->setSize({ width - (padding * 2), padding });
		filename->setPosition({ padding, padding * 2 });
		filename->setDefaultText("Filename");
		child->add(filename);
		yesbutton->setRenderer(theme.getRenderer("Button"));
		yesbutton->setPosition((width - buttonwidth) / 2, height - buttonheight);
		yesbutton->setText("save");
		yesbutton->setSize(buttonwidth, buttonheight);
		yesbutton->connect("pressed", [&]() {
			cout("savegame");
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
			this->createSavegame(filename);
		});
		child->add(yesbutton);
	}
}

void sfmlRenderer::ui_newgame(std::string message) {
	static tgui::Theme theme{ ASSETS_DIR + "guirenderer\\TransparentGrey.txt" };
	static const unsigned int width = 200;
	static const unsigned int height = 100;
	static const unsigned int posx = (screenWidth / 2) - (width / 2);
	static const unsigned int posy = ((screenHeight / 2) - 50) - (height / 2);
	static const unsigned int buttonwidth = width / 2;
	static const unsigned int buttonheight = height / 4;

	static bool constructed = FALSE;

	static auto child = tgui::ChildWindow::create();
	static auto label = tgui::Label::create();
	static auto yesbutton = tgui::Button::create();
	static auto nobutton = tgui::Button::create();

	if (displayingUI != nodisplay) {
		return;
	}
	else if (constructed == TRUE) {
		label->setText(message);
		if (child->isVisible()) {
			cout("setting newgame_ui visible FALSE");
			child->setVisible(FALSE);
			displayingUI = nodisplay;
		}
		else {
			cout("setting newgame_ui visible TRUE");
			child->setVisible(TRUE);
			displayingUI = display_update;
		}
	}
	else {
		cout("constructing ui_newgame");
		constructed = TRUE;
		displayingUI = display_noupdate;

		child->setResizable(FALSE);
		child->setPositionLocked(TRUE);
		child->setRenderer(theme.getRenderer("ChildWindow"));
		child->setSize(width, height);
		child->setPosition(posx, posy);
		child->setTitle("Confirm");
		child->connect("closed", [&]() {
			cout("child closed");
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		gui->add(child);
		label->setRenderer(theme.getRenderer("Label"));
		label->setText(message);
		label->setPosition(30, 10);
		label->setTextSize(15);
		child->add(label);
		yesbutton->setRenderer(theme.getRenderer("Button"));
		yesbutton->setPosition(0, height - buttonheight);
		yesbutton->setText("yes");
		yesbutton->setSize(buttonwidth, buttonheight);
		yesbutton->connect("pressed", [&]() {
			cout("yes newgame");
			game_status = chessfield::restart;
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		child->add(yesbutton);
		nobutton->setRenderer(theme.getRenderer("Button"));
		nobutton->setPosition(buttonwidth, height - buttonheight);
		nobutton->setText("no");
		nobutton->setSize(buttonwidth, buttonheight);
		nobutton->connect("pressed", [&]() {
			cout("no newgame");
			displayingUI = nodisplay;
			redraw = TRUE;
			child->setVisible(FALSE);
		});
		child->add(nobutton);
	}
}