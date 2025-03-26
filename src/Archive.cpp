#include "Archive.h"
#include "raygui-cpp/Controls/ComboBox.h"
#include "raygui-cpp/Controls/Panel.h"
#include "raygui-cpp/Controls/Button.h"
#include "png++/png.hpp"

PIX_RGB5 ColourToRGB5(raylib::Color& col) {
	PIX_RGB5 conv = { col.r / 8, col.g / 8, col.b / 8, 0 };
	return conv;
}

raylib::Color RGB5ToColour(PIX_RGB5* col) {
	return raylib::Color(col->r * 8, col->g * 8, col->b * 8);
}

Archive::Archive(std::string filepath, raygui::Layout& layout, Palette* palette) : pal(palette)
{
	unsigned char* file = LoadFileData(filepath.c_str(), &fileSize);

	Header* hdr = (Header*)file;
	unsigned char* seekHead = (unsigned char*)(hdr + 1);

	for (int i = 0; i < hdr->numImgs; i++) {
		ImgHeader* imgHdr = (ImgHeader*)seekHead;
		seekHead += sizeof(ImgHeader);

		if (imgHdr->type == 4) { // Load as palette
			CLUT clut;
			memcpy(&clut.header, imgHdr, sizeof(ImgHeader));
			clut.cols = (raylib::Color*)MemAlloc(sizeof(raylib::Color) * imgHdr->w);

			PIX_RGB5* col = (PIX_RGB5*)seekHead;

			for (int i = 0; i < imgHdr->w; i++) {
				clut.cols[i] = RGB5ToColour(col);
				col++;
			}

			seekHead = (unsigned char*)col;
			cluts.push_back(clut);
			fileStructure.push_back(1);
		}
		else {
			Sheet sheet;
			memcpy(&sheet.header, imgHdr, sizeof(ImgHeader));
			sheet.img = raylib::Image(imgHdr->w * 4, imgHdr->h);
			sheet.img.Format(PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
			int imgSize = (imgHdr->w * imgHdr->h) * 2;

			sheet.data = (unsigned char*)MemAlloc(imgSize);
			memcpy(sheet.data, seekHead, imgSize);
			seekHead += imgSize;

			sheets.push_back(sheet);
			fileStructure.push_back(0);
		}
	}

	UnloadFileData(file);
	
	sheetChoice = (raygui::ListView*)layout.GetControl("ImageSelect");
	sheetChoice->onChoose = std::bind(&Archive::SwapSheet, this, std::placeholders::_1);

	SetupPalette(layout);
	SetupSheets();

	raygui::Panel* viewPanel = (raygui::Panel*)layout.GetControl("WorkspaceFrame");
	raygui::Button* saveButton = (raygui::Button*)layout.GetControl("SaveButton");

	saveButton->onClicked = std::bind(&Archive::Save, this);

	viewport = raylib::RenderTexture(viewPanel->dimensions.width - 2, viewPanel->dimensions.height - 2);
	viewportDim = raylib::Rectangle(viewPanel->dimensions.GetPosition().x + 1, viewPanel->dimensions.GetPosition().y + 1, viewPanel->dimensions.width - 2, viewPanel->dimensions.height - 2);

	cam = raylib::Camera2D(viewPanel->dimensions.GetSize() / 2.f, raylib::Vector2(128,128));
}

Archive::~Archive()
{
	for (Sheet sheet : sheets) {
		MemFree(sheet.data);
	}
	for (CLUT clut : cluts) {
		MemFree(clut.cols);
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

	cursorPos = cam.GetScreenToWorld(raylib::Vector2(raylib::Mouse::GetPosition().x - viewportDim.x, (viewport.texture.height / 2.f - ((raylib::Mouse::GetPosition().y - viewport.texture.height / 2.f) - viewportDim.y)))) + raylib::Vector2(1, -1);

	if (raylib::Mouse::IsButtonDown(MOUSE_BUTTON_LEFT) 
		&& raylib::Rectangle(raylib::Vector2(), currentSheet.GetSize()).CheckCollision(cursorPos) 
		&& prevCursorPos != cursorPos
		&& viewportDim.CheckCollision(GetMousePosition())) {
		sheets[sheetChoice->activeIndex].DrawPixel(cursorPos, pal->GetSelectedColour());
		sheets[sheetChoice->activeIndex].ConvertToImage(pal);
		SwapSheet(sheetChoice->activeIndex);
		prevCursorPos = cursorPos;
	}
}
void Archive::Draw()
{
	viewport.BeginMode();
	cam.BeginMode();

	ClearBackground(RAYWHITE);
	currentSheet.Draw(0,0);

	if (viewportDim.CheckCollision(GetMousePosition()))
		DrawRectangle(cursorPos.x - 1, cursorPos.y + 1, 1, 1, Color{ 0,0,0,100 });

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
	if(index != -1)
		SwapPalette(pal->paletteSelect->activeIndex);
}
void Archive::Save()
{
	unsigned char* fileData = (unsigned char*)MemAlloc(fileSize);
	Header* hdr = (Header*)fileData;

	strncpy(hdr->magic, "TIP1", 4);
	hdr->numImgs = sheets.size() + cluts.size();

	ImgHeader* imgHdr = (ImgHeader*)(hdr + 1);
	int imgCnt = 0, palCnt = 0;
	for (int i = 0; i < fileStructure.size(); i++) {
		if (fileStructure[i]) {
			memcpy(imgHdr, &cluts[palCnt].header, sizeof(ImgHeader));
			PIX_RGB5* pix = (PIX_RGB5*)(imgHdr + 1);

			for (int j = 0; j < 16; j++) {
				*pix = ColourToRGB5(cluts[palCnt].cols[j]);
				pix++;
			}

			imgHdr = (ImgHeader*)pix;
			palCnt++;
		}
		else {
			memcpy(imgHdr, &sheets[imgCnt].header, sizeof(ImgHeader));
			memcpy(imgHdr + 1, sheets[imgCnt].data, (sheets[imgCnt].header.w * sheets[imgCnt].header.h) * 2);

			imgHdr = (ImgHeader*)((unsigned char*)(imgHdr + 1) + ((sheets[imgCnt].header.w * sheets[imgCnt].header.h) * 2));

			png::image<png::index_pixel_4> image(sheets[imgCnt].img.GetSize().x, sheets[imgCnt].img.GetSize().y);
			png::palette palette(16);
			for (int j = 0; j < 16; j++) {
				palette[j] = png::color(pal->colours[j].col.r, pal->colours[j].col.g, pal->colours[j].col.b);
			}
			image.set_palette(palette);

			IndexedPixel* pix = (IndexedPixel*)sheets[imgCnt].data;
			for (int y = 0; y < sheets[imgCnt].img.height; y++) {
				for (int x = 0; x < sheets[imgCnt].img.width / 2; x++) {
					image.set_pixel((x * 2), y, pix->pix0);
					image.set_pixel((x * 2) + 1, y, pix->pix1);
					pix++;
				}
			}
			image.write("Img" + std::to_string(i) + ".png");

			imgCnt++;
		}
	}

	SaveFileData("GUSSUN.Q", fileData, fileSize);

	MemFree(fileData);

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

void Archive::Sheet::DrawPixel(raylib::Vector2 pos, unsigned char palEntry)
{
	IndexedPixel* pix = (IndexedPixel*)data;
	pos.y = img.height / 2.f - (pos.y - img.height / 2.f);
	int offset = (int)pos.x + ((int)pos.y * img.width);

	if (offset % 2) {
		pix += offset / 2;
		pix->pix1 = palEntry;
	}
	else {
		pix += offset / 2;
		pix->pix0 = palEntry;
	}
}