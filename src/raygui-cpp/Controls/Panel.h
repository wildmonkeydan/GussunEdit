#pragma once
#include "../Control.h"

namespace raygui {
	class Panel : public Control
	{
	public:
		Panel(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string name);

		void Draw() override;

		std::string title;
	};
}