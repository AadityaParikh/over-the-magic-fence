#include <raylib.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 640

int main(int argc, char** argv) {
	InitWindow(SCREENWIDTH,SCREENHEIGHT,"Over The Magic Fence");

	SetTargetFPS(60);

	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(GetColor(0xFFFFFFFF));
			DrawText("Jack is a silly silly man",190,200,20,BLACK);
		EndDrawing();
	}
	CloseWindow();
}
