#pragma once
#include "../Control.h"
#include <functional>

namespace raygui {
	class ListView : public Control
	{
	public:
		ListView(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string listTxt, std::function<void(int)> chooseCallback);

		void Draw() override;

		std::string list;
		int scrollIndex = 0;
		int activeIndex = -1;
		std::function<void(int)> onChoose;
	};
}