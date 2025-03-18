#pragma once
#include "../Control.h"

namespace raygui {
	class DropdownBox : public Control
	{
	public:
		DropdownBox(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string listTxt);

		void Draw() override;

		std::string list;
		int activeIndex = 0;
	private:
		bool editMode = false;
	};
}