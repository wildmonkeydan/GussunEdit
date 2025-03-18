#include "Panel.h"

namespace raygui {
	Panel::Panel(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string name) : title(name)
	{
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void Panel::Draw()
	{
		GuiPanel(dimensions, title.c_str());
	}
}