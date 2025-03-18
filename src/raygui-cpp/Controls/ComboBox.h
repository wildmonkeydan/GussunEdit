#pragma once
#include "../Control.h"
#include <functional>

namespace raygui {
	class ComboBox : public Control
	{
	public:
		ComboBox(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string listTxt, std::function<void(int)> chooseCallback);

		void Draw() override;

		std::string list;
		int activeIndex = 0;
		std::function<void(int)> onChoose;
	};
}