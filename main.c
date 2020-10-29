#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv) {
	int screenWidth = 1920;
	int screenHeight = 1080;

	InitWindow(screenWidth,screenHeight,"Over The Magic Fence");

	SetTargetFPS(60);

	Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FIRST_PERSON);

	Model ring = LoadModel("Art/Models/ring.obj");

	Vector3 gizmoPos = {0.0f,0.0f,0.0f};

	Ray mouse = {0};
	
	float temp = 0.0f;
	int frame = 0;
	int coolDown = 0;

	while(!WindowShouldClose()) {

		UpdateCamera(&camera);

		frame++;

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&coolDown==0) {
			mouse = GetMouseRay(GetMousePosition(),camera);
			temp = mouse.direction.x;
			mouse.direction.x = mouse.direction.z;
			mouse.direction.z = (-1.0f)*temp; // rays seem to be rotated

			mouse.position.y *= 0.5f;
		}

		gizmoPos = mouse.position;

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);

				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawGizmo(gizmoPos);
				DrawModelWires(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,BLACK);
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);

				DrawRay(mouse,BLACK);

			EndMode3D();


		EndDrawing();
	}
	CloseWindow();
}

