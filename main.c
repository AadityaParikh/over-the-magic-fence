#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 640

int main(int argc, char** argv) {
	InitWindow(SCREENWIDTH,SCREENHEIGHT,"Over The Magic Fence");

	SetTargetFPS(60);

	Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_THIRD_PERSON);

	Vector3 pos = {0.0f,0.0f,0.0f};

	while(!WindowShouldClose()) {
		
		if(IsKeyDown(KEY_D)) {
			camera.target.x+=0.1f;
			camera.position.x+=0.1f;
		}
		if(IsKeyDown(KEY_A)) {
			camera.target.x-=0.1f;
			camera.position.x-=0.1f;
		}
		if(IsKeyDown(KEY_W)) {
			camera.target.z+=0.1f;
			camera.position.z+=0.1f;
		}
		if(IsKeyDown(KEY_S)) {
			camera.target.z-=0.1f;
			camera.position.z-=0.1f;
		}

		UpdateCamera(&camera);

		BeginDrawing();
			ClearBackground(WHITE);

			BeginMode3D(camera);

				DrawSphere(pos,1.0f,BLACK);
				DrawGrid(10,1.0f);

			EndMode3D();

		EndDrawing();
	}
	CloseWindow();
}
