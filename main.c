#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv) {
	//int screenWidth = GetMonitorWidth(0);
	//int screenHeight = GetMonitorHeight(0);
	int screenWidth = 840;
	int screenHeight = 600;

	InitWindow(screenWidth,screenHeight,"Over The Magic Fence");

	SetTargetFPS(60);

	Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 1.0f, 1.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FIRST_PERSON);

	Model ring = LoadModel("Art/Models/ring.obj");

	Vector3 spellPos = {0.0f,0.0f,0.0f};
	Vector3 spellDir = {0.0f,0.0f,0.0f};

	Ray mouse = {0};
	Ray test = {0.0f,1.0f,0.0f,1.0f,1.0f,0.0f};
	
	float temp = 0.0f;
	int frame = 0;
	int coolDown = 0;

	while(!WindowShouldClose()) {

		UpdateCamera(&camera);

		/*if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&coolDown==0) {
			mouse = GetMouseRay(GetMousePosition(),camera);
			//mouse.position.y *= 0.5f;
			spellPos = camera.position;
			spellDir = mouse.direction;


		}*/
		mouse = GetMouseRay(GetMousePosition(),camera);
		//mouse.position.y *= 0.5f;
		spellPos = mouse.position;
		spellDir = mouse.direction;

		temp = spellDir.x;
		spellDir.x = spellDir.z;
		spellDir.z = -temp;
		spellDir.y = 0.25f;

		frame++;
		test.direction.x+=0.001f;

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);

				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,BLACK);
				DrawGizmo((Vector3){0.0f,0.0f,0.0f});
				DrawSphere(spellPos,0.1f,BLACK);
				DrawSphere(Vector3Add(spellDir,spellPos),0.1f,BLACK);
				DrawSphere(spellDir,0.1f,BLACK);
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);

				DrawRay(mouse,BLACK);
				DrawRay(test,BLACK);
				

			EndMode3D();


		EndDrawing();
	}
	CloseWindow();
}

