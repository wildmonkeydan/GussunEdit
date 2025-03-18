#pragma once
#include "Control.h"
#include <map>

namespace raygui {
	class Layout
	{
	public:
		Layout(std::string filepath);
		~Layout();

		void Draw();
		Control* GetControl(std::string name);
	private:
		void ProcessRGLLine(std::string& line);
		raylib::Rectangle GetDimensionsFromParams(std::vector<std::string>& params);

		std::map<std::string,Control*> controls;
	};
}