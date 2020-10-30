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
	char* name;
	int coolDown;
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
	int spellCooldown = 0;
	int curSpell = 0;

	Spell spells[1];
	int numSpells = (sizeof(spells)/sizeof(spells[0]));

	spells[0].name = "fireball";
	spells[0].speed = 0.5f;
	spells[0].coolDown = 60; // 1 sec


	for(int j = 0;j<numSpells;j++) {
		for(int i = 0;i<4;i++) {
			char intStr[2];
			sprintf(intStr,"%d",i);
			char filename[]= "Art/Sprites/";
			strcat(filename,spells[j].name);
			strcat(filename,"/sprite_");
			strcat(filename,intStr);
			strcat(filename,".png");
			spells[j].sprites[i] = LoadTexture(filename);
		}
	}

	// raymath can't get linked for some systems for some reason
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


	char currentSpellName[50];
	char cooldownText[50];
	char intStrCooldown[50];
	while(!WindowShouldClose()) {

		UpdateCamera(&camera);
		frame++;
		if(spellCooldown > 0) spellCooldown--;

		screenDir = subVector3(camera.target,camera.position);
		screenDir = normVector3(screenDir);

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&spellCooldown==0) {
			spells[0].pos = camera.position;
			spells[0].dir = screenDir;

			spells[0].pos.y = 0.5f;

			spellCooldown =  spells[curSpell].coolDown;
		}

		for(int i = 0;i<numSpells;i++) {
			spells[i].pos = addVector3(spells[i].pos,scaleVector3(spells[i].dir,spells[i].speed));
			spells[i].sum = addVector3(spells[i].dir,spells[i].pos);
		}


		strcpy(currentSpellName,"Spell Active : ");
		strcat(currentSpellName,spells[curSpell].name);

		strcpy(cooldownText,"Cooldown : ");
		sprintf(intStrCooldown,"%d",spellCooldown);
		strcat(cooldownText,intStrCooldown);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);


				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);
				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);


				DrawBillboard(camera,spells[0].sprites[(frame/10)%4],spells[0].sum,1.0f,WHITE);
				

			EndMode3D();

			DrawFPS(10,10);
			DrawText(currentSpellName,10,30,20,LIME);
			DrawText(cooldownText,10,50,20,LIME);

		EndDrawing();
	}
	CloseWindow();

}

