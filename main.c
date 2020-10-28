#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 640

int main(int argc, char** argv) {
	InitWindow(SCREENWIDTH,SCREENHEIGHT,"Over The Magic Fence");

	SetTargetFPS(60);

	Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FIRST_PERSON);


	Vector3 pos = {0.0f,0.0f,0.0f};

	while(!WindowShouldClose()) {

		UpdateCamera(&camera);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);

				DrawGizmo(pos);
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){32.0f,32.0f},DARKGREEN);

				DrawRay((Ray){0.0f,0.0f,0.0f,0.5f,0.5f,0.0f},BLACK);

			EndMode3D();


		EndDrawing();
	}
	CloseWindow();
}
