#include "ComboBox.h"

namespace raygui {
	ComboBox::ComboBox(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string listTxt, std::function<void(int)> chooseCallback) : list(listTxt), onChoose(chooseCallback)
	{
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void ComboBox::Draw()
	{
		int currentIndex = activeIndex;
		GuiComboBox(dimensions, list.c_str(), &activeIndex);

		if (currentIndex != activeIndex && onChoose)
			onChoose(activeIndex);
	}
}