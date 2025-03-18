#include "ListView.h"

namespace raygui {
	ListView::ListView(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string listTxt, std::function<void(int)> chooseCallback) : list(listTxt), onChoose(chooseCallback)
	{
		dimensions = _dimensions;
		id = _id;
		active = _active;
	}
	void ListView::Draw()
	{
		int currentIndex = activeIndex;
		GuiListView(dimensions, list.c_str(), &scrollIndex, &activeIndex);

		if (currentIndex != activeIndex && onChoose)
			onChoose(activeIndex);
	}
}