#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


typedef struct Spell {
	Vector3 pos;
	Vector3 dir;
	Vector3 sum;
	float speed;
} Spell;

int main(int argc, char** argv) {
	int screenWidth = GetMonitorWidth(0);
	int screenHeight = GetMonitorHeight(0);

	InitWindow(screenWidth,screenHeight,"Over The Magic Fence");

	SetTargetFPS(60);

	Camera3D camera = { 0 };
    camera.position = (Vector3){ 1.0f, 1.0f, 0.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.type = CAMERA_PERSPECTIVE;                   // Camera mode type
	SetCameraMode(camera, CAMERA_FIRST_PERSON);

	Model ring = LoadModel("Art/Models/ring.obj");

	Vector3 zeroV3 = {0.0f,0.0f,0.0f};

	Ray mouse = {0};
	
	float temp = 0.0f;
	int frame = 0;
	int coolDown = 0;

	Spell fireball = {zeroV3,zeroV3,zeroV3,0.1f};

	Texture2D test = LoadTexture("Art/fireball/sprite_0.png");

	Vector3 addVector3(Vector3 v1,Vector3 v2) {
		Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
		return result;
	}
	Vector3 subVector3(Vector3 v1,Vector3 v2) {
		Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
		return result;
	}
	Vector3 scaleVector3(Vector3 v1,float scalar) {
		Vector3 result = { v1.x * scalar, v1.y * scalar, v1.z * scalar };
		return result;
	}
	Vector3 lenVector3(Vector3 vec) {
		//Vector3 result = 
	}

	while(!WindowShouldClose()) {

		UpdateCamera(&camera);

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&coolDown==0) {
			mouse = GetMouseRay(GetMousePosition(),camera);
			fireball.pos = mouse.position;
			fireball.dir = mouse.direction;

			temp = fireball.dir.x;
			fireball.dir.x = fireball.dir.z;
			fireball.dir.z = -temp; // rotating it
			fireball.dir.y = 0.0f;
			fireball.pos.y = 0.5f;
			fireball.sum = addVector3(fireball.dir,fireball.pos);

		}

		frame++;

		fireball.pos = addVector3(fireball.pos,scaleVector3(fireball.dir,0.1f)); // TODO make it not hardcoded
		fireball.sum = addVector3(fireball.dir,fireball.pos);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);

				//DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);
				DrawSphere(fireball.pos,0.1f,BLACK);
				DrawSphere(fireball.sum,0.1f,BLACK);
				DrawSphere(fireball.dir,0.1f,BLACK);
				DrawSphereWires(camera.target,0.1f,16,16,BLACK);
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);

				DrawRay((Ray){fireball.pos,fireball.dir},BLACK);
				DrawRay((Ray){camera.position,subVector3()},BLACK);

				DrawBillboard(camera,test,fireball.sum,1.0f,WHITE);
				

			EndMode3D();


		EndDrawing();
	}
	CloseWindow();


}

