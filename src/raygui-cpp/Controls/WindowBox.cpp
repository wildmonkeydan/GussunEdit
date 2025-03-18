#include "WindowBox.h"

namespace raygui {
	WindowBox::WindowBox(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string _title) : title(_title) {
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void WindowBox::Draw()
	{
		GuiWindowBox(dimensions, title.c_str());
	}
}