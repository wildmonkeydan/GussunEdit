#include "Layout.h"
#include <sstream>

#include "Controls/WindowBox.h"
#include "Controls/Panel.h"
#include "Controls/Button.h"
#include "Controls/DropdownBox.h"
#include "Controls/Label.h"
#include "Controls/ListView.h"
#include "Controls/ComboBox.h"

using namespace raylib;

namespace raygui {
	Layout::Layout(std::string filepath)
	{
		std::string layout = LoadFileText(filepath);
		std::istringstream iss(layout);
		std::string line;

		while (std::getline(iss, line)) {
			ProcessRGLLine(line);
		}
		
	}
	Layout::~Layout()
	{
		for (auto control : controls) {
			delete control.second;
		}
	}
	void Layout::Draw()
	{
		for (auto control : controls) {
			control.second->Draw();
		}
	}
	Control* Layout::GetControl(std::string name)
	{
		return controls[name];
	}
	void Layout::ProcessRGLLine(std::string& line)
	{
		std::vector<std::string> params = TextSplit(line, ' ');

		if (params[0] == "c") {
			unsigned int id = atoi(params[1].c_str());
			Control::GuiControlType type = (Control::GuiControlType)atoi(params[2].c_str());
			std::pair<std::string, Control*> control(params[3],nullptr);
			std::string text = params[9];
			
			if (params.size() > 10) {
				for (int i = 10; i < params.size(); i++) {
					text += (" " + params[i]);
				}
			}

			switch (type)
			{
			case Control::GuiControlType::GUI_WINDOWBOX:
				control.second = new WindowBox(GetDimensionsFromParams(params), id, true, text);
				break;
			case Control::GuiControlType::GUI_GROUPBOX:
				break;
			case Control::GuiControlType::GUI_LINE:
				break;
			case Control::GuiControlType::GUI_PANEL:
				control.second = new Panel(GetDimensionsFromParams(params), id, true, text);
				break;
			case Control::GuiControlType::GUI_LABEL:
				control.second = new Label(GetDimensionsFromParams(params), id, true, text);
				break;
			case Control::GuiControlType::GUI_BUTTON:
				control.second = new Button(GetDimensionsFromParams(params), id, true, text, nullptr, std::vector<int>());
				break;
			case Control::GuiControlType::GUI_LABELBUTTON:
				break;
			case Control::GuiControlType::GUI_CHECKBOX:
				break;
			case Control::GuiControlType::GUI_TOGGLE:
				break;
			case Control::GuiControlType::GUI_TOGGLEGROUP:
				break;
			case Control::GuiControlType::GUI_COMBOBOX:
				control.second = new ComboBox(GetDimensionsFromParams(params), id, true, text, nullptr);
				break;
			case Control::GuiControlType::GUI_DROPDOWNBOX:
				control.second = new DropdownBox(GetDimensionsFromParams(params), id, true, text);
				break;
			case Control::GuiControlType::GUI_TEXTBOX:
				break;
			case Control::GuiControlType::GUI_TEXTBOXMULTI:
				break;
			case Control::GuiControlType::GUI_VALUEBOX:
				break;
			case Control::GuiControlType::GUI_SPINNER:
				break;
			case Control::GuiControlType::GUI_SLIDER:
				break;
			case Control::GuiControlType::GUI_SLIDERBAR:
				break;
			case Control::GuiControlType::GUI_PROGRESSBAR:
				break;
			case Control::GuiControlType::GUI_STATUSBAR:
				break;
			case Control::GuiControlType::GUI_SCROLLPANEL:
				break;
			case Control::GuiControlType::GUI_LISTVIEW:
				control.second = new ListView(GetDimensionsFromParams(params), id, true, text, nullptr);
				break;
			case Control::GuiControlType::GUI_COLORPICKER:
				break;
			case Control::GuiControlType::GUI_DUMMYREC:
				break;
			default:
				break;
			}

			controls.insert(control);
		}
	}
	raylib::Rectangle Layout::GetDimensionsFromParams(std::vector<std::string>& params)
	{
		return raylib::Rectangle(atoi(params[4].c_str()), atoi(params[5].c_str()), atoi(params[6].c_str()), atoi(params[7].c_str()));
	}
}