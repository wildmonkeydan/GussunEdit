#pragma once
#include <string>
#include "raylib-cpp.hpp"
#include "raygui.h"

namespace raygui {
	class Control
	{
	public:
        // Controls type id
        // WARNING: Do not change those values to avoid breaking all layouts created
        // In case a control type needs to be removed, just remove it without changing values
        // In case a control type needs to be added, just add at the end or use an empty space value
        // WARNING: Controls names array (defined below) MUST be aligned with GuiControlType enum values
        typedef enum {
            GUI_WINDOWBOX = 0,
            GUI_GROUPBOX = 1,
            GUI_LINE = 2,
            GUI_PANEL = 3,
            GUI_LABEL = 4,
            GUI_BUTTON = 5,
            GUI_LABELBUTTON = 6,
            GUI_CHECKBOX = 7,
            GUI_TOGGLE = 8,
            GUI_TOGGLEGROUP = 9,
            GUI_COMBOBOX = 10,
            GUI_DROPDOWNBOX = 11,
            GUI_TEXTBOX = 12,
            GUI_TEXTBOXMULTI = 13,
            GUI_VALUEBOX = 14,
            GUI_SPINNER = 15,
            GUI_SLIDER = 16,
            GUI_SLIDERBAR = 17,
            GUI_PROGRESSBAR = 18,
            GUI_STATUSBAR = 19,
            GUI_SCROLLPANEL = 20,
            GUI_LISTVIEW = 21,
            GUI_COLORPICKER = 22,
            GUI_DUMMYREC = 23
        } GuiControlType;

		virtual void Draw() = 0;

		raylib::Rectangle dimensions;
	protected:
		GuiControlType type;
		unsigned int id;
        bool active = true;
	};
}