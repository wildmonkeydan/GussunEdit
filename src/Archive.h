#pragma once
#include "raygui-cpp/Layout.h"
#include "raylib-cpp.hpp"
#include "Palette.h"

class Archive
{
public:
	struct Header {
		char magic[4]; // Should be TIP1
		int numImgs;
		unsigned int params;
		unsigned int params2;
		short x, y, w, h;
	};

	struct Sheet {
		unsigned char* data;
		raylib::Image img;
	};

	Archive(std::string filepath, raygui::Layout& layout, Palette* palette);

	void Update();
	void Draw();
private:
	Palette* pal;
	raylib::Texture currentSheet;
	std::vector<Sheet> sheets;
};