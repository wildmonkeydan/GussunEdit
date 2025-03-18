#include "Label.h"

namespace raygui {
	Label::Label(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string txt) : text(txt)
	{
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void Label::Draw()
	{
		GuiLabel(dimensions, text.c_str());
	}
}