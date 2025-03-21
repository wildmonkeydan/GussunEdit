#pragma once
#include "raygui-cpp/Layout.h"
#include "raygui-cpp/Controls/ListView.h"
#include "raylib-cpp.hpp"
#include "Palette.h"


// From img2tim https://github.com/Lameguy64/img2tim/blob/master/tim.h#L69
// RGB5A1 pixel format struct
typedef struct {
	unsigned short r : 5;
	unsigned short g : 5;
	unsigned short b : 5;
	unsigned short i : 1;
} PIX_RGB5;

class Archive
{
public:
	struct Header {
		char magic[4]; // Should be TIP1
		int numImgs;
	};

	struct ImgHeader {
		int dummy, dummy2;
		short x, y, w, h;
	};

	struct Sheet {
		unsigned char* data = nullptr;
		raylib::Image img;
		ImgHeader header;

		void ConvertToImage(Palette* pal);
		void DrawPixel(raylib::Vector2 pos, unsigned char palEntry);
	};

	struct CLUT {
		ImgHeader header;
		raylib::Color cols[16];
	};

	struct IndexedPixel {
		unsigned char pix0 : 4;
		unsigned char pix1 : 4;
	};

	Archive(std::string filepath, raygui::Layout& layout, Palette* palette);
	~Archive();

	void Update();
	void Draw();
private:
	void SetupPalette(raygui::Layout& layout);
	void SetupSheets();

	void SwapPalette(int index);
	void SwapSheet(int index);
	void Save();

	Palette* pal = nullptr;
	raylib::Texture currentSheet;
	raylib::RenderTexture viewport;
	raylib::Camera2D cam;
	raylib::Rectangle viewportDim;

	raylib::Vector2 cursorPos;
	raylib::Vector2 prevCursorPos;

	std::vector<Sheet> sheets;
	std::vector<CLUT> cluts;
	std::vector<int> fileStructure;
	int fileSize = 0;

	raygui::ListView* sheetChoice;

	const float zoomPower = 4.f;
};
