#pragma once
#include "../Control.h"

namespace raygui {
	class Label : public Control
	{
	public:
		Label(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string txt);

		void Draw() override;

		std::string text;
	};
}