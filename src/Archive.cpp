#include "Archive.h"
#include "raygui-cpp/Controls/ComboBox.h"
#include "raygui-cpp/Controls/Panel.h"

PIX_RGB5 ColourToRGB5(raylib::Color& col) {
	PIX_RGB5 conv = { col.r / 8, col.g / 8, col.b / 8, 0 };
	return conv;
}

raylib::Color RGB5ToColour(PIX_RGB5* col) {
	return raylib::Color(col->r * 8, col->g * 8, col->b * 8);
}

Archive::Archive(std::string filepath, raygui::Layout& layout, Palette* palette) : pal(palette)
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
			sheet.img = raylib::Image(imgHdr->w * 4, imgHdr->h);
			sheet.img.Format(PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			int imgSize = (imgHdr->w * imgHdr->h) * 2;

			sheet.data = (unsigned char*)MemAlloc(imgSize);
			memcpy(sheet.data, seekHead, imgSize);
			seekHead += imgSize;

			sheets.push_back(sheet);
		}
	}

	UnloadFileData(file);
	
	sheetChoice = (raygui::ListView*)layout.GetControl("ImageSelect");
	sheetChoice->onChoose = std::bind(&Archive::SwapSheet, this, std::placeholders::_1);

	SetupPalette(layout);
	SetupSheets();

	raygui::Panel* viewPanel = (raygui::Panel*)layout.GetControl("WorkspaceFrame");

	viewport = raylib::RenderTexture(viewPanel->dimensions.width - 2, viewPanel->dimensions.height - 2);
	viewportDim = raylib::Rectangle(viewPanel->dimensions.GetPosition().x + 1, viewPanel->dimensions.GetPosition().y + 1, viewPanel->dimensions.width - 2, viewPanel->dimensions.height - 2);

	cam = raylib::Camera2D(viewPanel->dimensions.GetSize() / 2.f, raylib::Vector2(128,128));
}

Archive::~Archive()
{
	for (Sheet sheet : sheets) {
		MemFree(sheet.data);
	}
}

void Archive::Update()
{
	float scroll = GetMouseWheelMove();

	if (scroll != 0.f) {
		cam.zoom += scroll / zoomPower;
	}

	if (raylib::Vector2(GetMouseDelta()) != raylib::Vector2() && raylib::Mouse::IsButtonDown(MOUSE_BUTTON_MIDDLE)) {
		cam.offset = raylib::Vector2(raylib::Mouse::GetDelta().x, raylib::Mouse::GetDelta().y * -1) + cam.offset;
	}

	cursorPos = cam.GetScreenToWorld(raylib::Vector2(raylib::Mouse::GetPosition().x - viewportDim.x, (viewport.texture.height / 2.f - ((raylib::Mouse::GetPosition().y - viewport.texture.height / 2.f) - viewportDim.y))));
}
void Archive::Draw()
{
	viewport.BeginMode();
	cam.BeginMode();

	ClearBackground(RAYWHITE);
	currentSheet.Draw(0,0);

	if (viewportDim.CheckCollision(GetMousePosition()))
		DrawRectangle(cursorPos.x, cursorPos.y, 1, 1, Color{ 0,0,0,100 });

	cam.EndMode();
	viewport.EndMode();

	viewport.GetTexture().Draw(viewportDim.GetPosition());
}

void Archive::SetupPalette(raygui::Layout& layout)
{
	raygui::ComboBox* comboBox = (raygui::ComboBox*)layout.GetControl("PaletteSelect");
	pal->paletteSelect = comboBox;

	raylib::Vector2 offset(pal->paletteSelect->dimensions.x + pal->paletteSelect->dimensions.width + 72, pal->paletteSelect->dimensions.y);
	raylib::Vector2 size(24, 24);
	for (int i = 0; i < 16; i++) {
		pal->colours[i].rect = raylib::Rectangle(offset, size);
		offset += raylib::Vector2(24, 0);
	}

	std::string list;
	for (int i = 0; i < cluts.size(); i++) {
		list += std::to_string(i + 1) + ";";
	}
	list.erase(list.end() - 1);

	comboBox->list = list;

	comboBox->onChoose = std::bind(&Archive::SwapPalette, this, std::placeholders::_1);
	SwapPalette(0);
}

void Archive::SetupSheets()
{
	std::string list;

	for (int i = 0; i < sheets.size(); i++) {
		sheets[i].ConvertToImage(pal);
		list += std::to_string(i + 1) + ";";
	}
	list.erase(list.end() - 1);

	currentSheet = sheets[0].img.LoadTexture();
	sheetChoice->list = list;
}

void Archive::SwapPalette(int index)
{
	for (int i = 0; i < 16; i++) {
		pal->colours[i].col = cluts[index].cols[i];
	}
	sheets[sheetChoice->activeIndex].ConvertToImage(pal);
	currentSheet = sheets[sheetChoice->activeIndex].img.LoadTexture();
}

void Archive::SwapSheet(int index)
{
	SwapPalette(pal->paletteSelect->activeIndex);
}

void Archive::Sheet::ConvertToImage(Palette* pal)
{
	IndexedPixel* pix = (IndexedPixel*)data;
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width / 2; x++) {
			img.DrawPixel(x * 2, y, pal->colours[pix->pix0].col);
			img.DrawPixel((x * 2) + 1, y, pal->colours[pix->pix1].col);
			pix++;
		}
	}
	img.Rotate(180);
	img.FlipHorizontal();
}
