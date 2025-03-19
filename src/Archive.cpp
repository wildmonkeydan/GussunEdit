#include "Archive.h"
#include "raygui-cpp/Controls/ComboBox.h"
#include "raygui-cpp/Controls/Panel.h"

using namespace raylib;

PIX_RGB5 ColourToRGB5(raylib::Color& col) {
	PIX_RGB5 conv = { col.r / 8, col.g / 8, col.b / 8, 0 };
	return conv;
}

raylib::Color RGB5ToColour(PIX_RGB5* col) {
	return raylib::Color(col->r * 8, col->g * 8, col->b * 8);
}

Archive::Archive(std::string filepath, raygui::Layout& layout, Palette* palette)
{
	int fileSize = 0;
	unsigned char* file = LoadFileData(filepath.c_str(), &fileSize);

	Header* hdr = (Header*)file;
	unsigned char* seekHead = (unsigned char*)(hdr + 1);

	for (int i = 0; i < hdr->numImgs; i++) {
		ImgHeader* imgHdr = (ImgHeader*)seekHead;
		seekHead += sizeof(ImgHeader);

		if (imgHdr->h == 1) { // Load as palette
			CLUT clut;
			PIX_RGB5* col = (PIX_RGB5*)seekHead;

			for (int i = 0; i < imgHdr->w; i++) {
				clut.cols[i] = RGB5ToColour(col);
				col++;
			}

			seekHead = (unsigned char*)col;
			cluts.push_back(clut);
		}
		else {
			Sheet sheet;
			sheet.img = raylib::Image(imgHdr->w, imgHdr->h);
			int imgSize = (imgHdr->w * imgHdr->h) * 2;

			sheet.data = (unsigned char*)MemAlloc(imgSize);
			memcpy(sheet.data, seekHead, imgSize);
			seekHead += imgSize;

			sheets.push_back(sheet);
		}
	}

	UnloadFileData(file);

	SetupPalette(layout);
	SetupSheets();

	raygui::Panel* viewPanel = (raygui::Panel*)layout.GetControl("Workspace");

	viewport = raylib::RenderTexture(viewPanel->dimensions.width, viewPanel->dimensions.height);
	viewportPos = viewPanel->dimensions.GetPosition();
}

Archive::~Archive()
{
	for (Sheet sheet : sheets) {
		MemFree(sheet.data);
	}
}

void Archive::Update()
{
}
void Archive::Draw()
{
	viewport.BeginMode();
	currentSheet.Draw();
	viewport.EndMode();

	viewport.GetTexture().Draw(viewportPos);
}

void Archive::SetupPalette(raygui::Layout& layout)
{
	raygui::ComboBox* comboBox = (raygui::ComboBox*)layout.GetControl("PaletteSelect");
	pal->SetComboBox(comboBox);

	comboBox->onChoose = std::bind(&Archive::SwapPalette, this, std::placeholders::_1);
	SwapPalette(0);
}

void Archive::SetupSheets()
{
	for (int i = 0; i < sheets.size(); i++) {
		IndexedPixel* pix = (IndexedPixel*)sheets[i].data;
		for (int y = 0; y < sheets[i].img.height; y++) {
			for (int x = 0; x < sheets[i].img.width * 2; x++) {
				sheets[i].img.DrawPixel(x * 2, y, pal->colours[pix->pix0]);
				sheets[i].img.DrawPixel((x * 2) + 1, y, pal->colours[pix->pix1]);
			}
		}
	}

	currentSheet = sheets[0].img.LoadTexture();
}

void Archive::SwapPalette(int index)
{
	memcpy(pal->colours, cluts[index].cols, sizeof(raylib::Color) * 16);
}
