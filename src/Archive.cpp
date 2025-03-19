#include "Archive.h"

using namespace raylib;

Archive::Archive(std::string filepath, raygui::Layout& layout, Palette* palette)
{
	int fileSize = 0;
	unsigned char* file = LoadFileData(filepath.c_str(), &fileSize);

	Header* hdr = (Header*)file;
	unsigned char* seekHead = (unsigned char*)(hdr + 1);

	for (int i = 0; i < hdr->numImgs; i++) {

	}
}

void Archive::Update()
{
}
void Archive::Draw()
{
}
