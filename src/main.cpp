#include "raylib-cpp.hpp"
#define RAYGUI_IMPLEMENTATION
#include "raygui-cpp/Layout.h"
#include "raygui-cpp/Controls/Button.h"
#include "Archive.h"
#include "tinyfiledialogs/tinyfiledialogs.h"

void TestButton() {
    TraceLog(LOG_INFO, "Hii");
}

int main() {
    int screenWidth = 1224;
    int screenHeight = 816;

    raylib::Window window(screenWidth, screenHeight, "GussunEdit");
    raygui::Layout layout("GussunEdit.rgl");
    Palette pal;

    int filterCount = 0;
    const char** none = (const char**)TextSplit("*.Q;*.P", ';', &filterCount);
    const char* fName = tinyfd_openFileDialog("Open a file..", GetApplicationDirectory(), 0, none, "ZGO Graphics Files (.Q,.P)", filterCount);

    Archive arch(fName, layout, &pal);

    SetTargetFPS(60);

    while (!window.ShouldClose())
    {
        arch.Update();
        pal.Update();

        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        layout.Draw();
        arch.Draw();
        pal.Draw();

        EndDrawing();
    }

    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}