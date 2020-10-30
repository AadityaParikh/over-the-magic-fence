#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


typedef struct Spell {
	Vector3 pos;
	Vector3 dir;
	Vector3 sum;
	float speed;
	Texture2D sprites[5];
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
	Vector3 screenDir = zeroV3; // normalized vector of direction screen is pointing in

	Ray mouse = {0};
	
	float temp = 0.0f;
	int frame = 0;

	Spell fireball;
	fireball.speed = 0.5f;
	for(int i = 0;i<4;i++) {
		char intStr[2];
		sprintf(intStr,"%d",i);
		char filename[]= "Art/fireball/sprite_";
		strcat(filename,intStr);
		strcat(filename,".png");
		fireball.sprites[i] = LoadTexture(filename);
	}

	Vector3 addVector3(Vector3 v1,Vector3 v2) { // add
		Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
		return result;
	}
	Vector3 subVector3(Vector3 v1,Vector3 v2) { // subtract
		Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
		return result;
	}
	Vector3 scaleVector3(Vector3 vec,float scalar) { // scale
		Vector3 result = { vec.x * scalar, vec.y * scalar, vec.z * scalar };
		return result;
	}
	Vector3 normVector3(Vector3 vec) { // normalize
		float mag = sqrtf((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
		if(mag==0.0f) return vec;
		Vector3 result = {vec.x/mag,vec.y/mag,vec.z/mag};
		return result;
	}


	//player vars (player struct isn't necessary cause there's only 1 player);
	int spellCooldown = 0;
	int curSpell = 0;

	Spell spells[1];

	while(!WindowShouldClose()) {

		UpdateCamera(&camera);
		frame++;

		screenDir = subVector3(camera.target,camera.position);
		screenDir.y = 0;
		screenDir = normVector3(screenDir);

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&spellCooldown==0) {
			fireball.pos = camera.position;
			fireball.dir = screenDir;

			fireball.pos.y = 0.5f;
		}

		fireball.pos = addVector3(fireball.pos,scaleVector3(fireball.dir,fireball.speed)); // TODO make it not hardcoded
		fireball.sum = addVector3(fireball.dir,fireball.pos);



		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);

				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);
				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);

				DrawBillboard(camera,fireball.sprites[(frame/10)%4],fireball.sum,1.0f,WHITE);
				

			EndMode3D();


		EndDrawing();
	}
	CloseWindow();

}

