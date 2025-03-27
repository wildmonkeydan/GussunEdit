#include "Palette.h"

Palette::Palette()
{
}

Palette::~Palette()
{
}

void Palette::Update()
{
	for (int i = 0; i < palLen; i++) {
		if (colours[i].rect.CheckCollision(GetMousePosition()) && IsMouseButtonPressed(0))
			selectedColour = i;
	}
}
void Palette::Draw()
{
	for (int i = 0; i < palLen; i++) {
		colours[i].rect.Draw(colours[i].col);
		if (i == selectedColour)
			colours[i].rect.DrawLines(RED);
	}
}
int Palette::GetSelectedColour()
{
	return selectedColour;
}

void Palette::SetComboBox(raygui::ComboBox* combo)
{
}
