#pragma once
#include "../Control.h"

namespace raygui {
	class WindowBox : public Control
	{
	public:
		WindowBox(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string _title);

		void Draw() override;

		std::string title;
	};
}