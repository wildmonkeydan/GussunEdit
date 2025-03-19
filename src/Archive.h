#pragma once
#include "raygui-cpp/Layout.h"
#include "Palette.h"

class Archive
{
public:
	struct Header {
		char magic[4]; // Should be TIP1
		int numImgs;
	};

	Archive(std::string filepath, raygui::Layout& layout, Palette* palette);

	void Update();
	void Draw();
private:
	Palette* pal;
};