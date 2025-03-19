#pragma once
#include "raygui-cpp/Controls/ComboBox.h"

class Palette
{
public:
	Palette();

	void Update();
	void Draw();
	int GetSelectedColour();

	void SetComboBox(raygui::ComboBox* combo);

	raylib::Color colours[16];
private:
	raygui::ComboBox* paletteSelect = nullptr;
	int selectedColour = 0;
};