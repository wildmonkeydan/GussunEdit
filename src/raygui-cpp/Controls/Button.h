#pragma once
#include "../Control.h"
#include <functional>

namespace raygui {
	class Button : public Control
	{
	public:
		Button(raylib::Rectangle _dimensions, unsigned int _id, bool _active, std::string txt, std::function<void()> clickCallback, std::vector<int> shortcutKeys);

		void Draw() override;

		std::string text;
		std::function<void()> onClicked;
		std::vector<int> shortcut;
	private:
		bool canShortcut = true;
	};
}