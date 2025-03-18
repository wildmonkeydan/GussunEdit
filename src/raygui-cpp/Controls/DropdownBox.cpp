#include "DropdownBox.h"

namespace raygui {
	DropdownBox::DropdownBox(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string listTxt) : list(listTxt)
	{
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void DropdownBox::Draw()
	{
		if (GuiDropdownBox(dimensions, list.c_str(), &activeIndex, editMode))
			editMode = !editMode;
	}
}