#pragma once
#include "raygui-cpp/Controls/ComboBox.h"

class Palette
{
public:
	struct ColourBox {
		raylib::Color col;
		raylib::Rectangle rect;
	};

	Palette();
	~Palette();

	void Update();
	void Draw();
	int GetSelectedColour();

	void SetComboBox(raygui::ComboBox* combo);
	ColourBox* colours = nullptr;
	raygui::ComboBox* paletteSelect = nullptr;
private:
	int selectedColour = 0;
};