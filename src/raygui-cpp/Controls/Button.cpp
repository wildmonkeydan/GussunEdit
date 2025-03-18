#include "Button.h"

namespace raygui {
	Button::Button(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string txt, std::function<void()> clickCallback, std::vector<int> shortcutKeys) : text(txt), onClicked(clickCallback), shortcut(shortcutKeys)
	{
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void Button::Draw()
	{
		if (GuiButton(dimensions, text.c_str()) && onClicked != nullptr)
			onClicked();

		if (shortcut.size()) {
			if (canShortcut) {
				int pressed = 0;

				for (int i = 0; i < shortcut.size(); i++)
					pressed += raylib::Keyboard::IsKeyDown(shortcut[i]);

				if (pressed == shortcut.size()) {
					onClicked();
					canShortcut = false;
				}
			}
			else {
				int released = 0;

				for (int i = 0; i < shortcut.size(); i++)
					released += !raylib::Keyboard::IsKeyDown(shortcut[i]);

				if (released == shortcut.size())
					canShortcut = true;
			}
		}
	}
}