#include "raylib-cpp.hpp"
#define RAYGUI_IMPLEMENTATION
#include "raygui-cpp/Layout.h"
#include "raygui-cpp/Controls/Button.h"

void TestButton() {
    TraceLog(LOG_INFO, "Hii");
}

int main() {
    int screenWidth = 1224;
    int screenHeight = 816;

    raylib::Window window(screenWidth, screenHeight, "GussunEdit");
    raygui::Layout layout("GussunEdit.rgl");

    raygui::Button* button = (raygui::Button*)layout.GetControl("OpenFileButton");
    button->onClicked = TestButton;
    std::vector<int> shortcut = { KEY_LEFT_CONTROL,KEY_O };
    button->shortcut = shortcut;

    SetTargetFPS(60);

    while (!window.ShouldClose())
    {
        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        layout.Draw();

        EndDrawing();
    }

    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}