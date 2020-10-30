#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


typedef struct Spell {
	Vector3 pos;
	Vector3 dir;
	Vector3 init;
	float speed;
	Texture2D sprites[5];
	char* name;
	int coolDown;
	float len;
} Spell;

int main(int argc, char** argv) {
	int screenWidth = GetMonitorWidth(0);
	int screenHeight = GetMonitorHeight(0);

	InitWindow(screenWidth,screenHeight,"Over The Magic Fence");

	SetTargetFPS(60);

	Camera3D camera = { 0 };
    camera.position = (Vector3){ 1.0f, 1.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; 
    camera.fovy = 45.0f;
    camera.type = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_FIRST_PERSON);

	Model ring = LoadModel("Art/Models/ring.obj");

	Vector3 zeroV3 = {0.0f,0.0f,0.0f};
	Vector3 screenDir = zeroV3; // normalized vector of direction screen is pointing in

	Ray mouse = {0};
	
	float temp = 0.0f;
	int frame = 0;
	int spellCooldown = 0;
	int curSpell = 0;
	bool spellActive = 0;

	Spell spells[1] = {0};
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
	float magVector3(Vector3 vec) { // normalize
		float result = sqrtf((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
		return result;
	}


	char debugText[12][50] = {0};
	while(!WindowShouldClose()) {

		UpdateCamera(&camera);
		frame++;
		if(spellCooldown > 0) spellCooldown--;

		screenDir = subVector3(camera.target,camera.position);
		screenDir = normVector3(screenDir);

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&spellCooldown==0&&spellActive==0) {
			spells[0].pos = camera.position;
			spells[0].init = camera.position;
			spells[0].dir = screenDir;

			spells[0].pos.y = 0.5f;

			spellCooldown =  spells[curSpell].coolDown;
			spellActive = 1;
		}

		for(int i = 0;i<numSpells;i++) {
			spells[i].pos = addVector3(spells[i].pos,scaleVector3(spells[i].dir,spells[i].speed));
			spells[i].len = magVector3(subVector3(spells[i].pos,spells[i].init));

			if(	(i == curSpell && spellActive) &&
				(spells[i].pos.y<=0.1f || // under the ground
				spells[i].pos.x>=100.0f || // out of bounds x
				spells[i].pos.y>=100.0f || // out of bounds y
				spells[i].pos.z>=100.0f || // out of bounds z
				spells[i].len > 75.0f)
			) {


				spells[i].dir = zeroV3;
				spells[i].pos = (Vector3){0,1,0};
				spellActive = 0;
			}

			if(i != curSpell || !spellActive) {
				spells[i].pos = camera.position;
			}
		}

		sprintf(debugText[0],"%s : %d","FPS",GetFPS());
		sprintf(debugText[1],"%s : %s","Spell Active",spellActive?"Yes":"No");
		sprintf(debugText[2],"%s : %d","Cooldown",spellCooldown);

		sprintf(debugText[3],"%s : ","Current Spell Info");
		sprintf(debugText[4],"\t%s : %s","Name",spells[curSpell].name);
		sprintf(debugText[5],"\t%s : %f,%f,%f","Pos",spells[curSpell].pos.x,spells[curSpell].pos.y,spells[curSpell].pos.z);
		sprintf(debugText[6],"\t%s : %f,%f,%f","Dir",spells[curSpell].dir.x,spells[curSpell].dir.y,spells[curSpell].dir.z);
		sprintf(debugText[7],"\t%s : %d","MaxCooldown",spells[curSpell].coolDown);
		sprintf(debugText[8],"\t%s : %f","Speed",spells[curSpell].speed);
		sprintf(debugText[9],"\t%s : %f","Distance",spells[curSpell].len);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);
				
				
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);
				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);
				
				DrawBillboard(camera,spells[0].sprites[(frame/10)%4],spells[0].pos,1.0f,WHITE);
				
				
			EndMode3D();

			for(int i = 0;i<(sizeof(debugText)/sizeof(debugText[0]));i++) {
				DrawText(debugText[i],10,10+(i*20),20,BLACK);
			}

		EndDrawing();
	}
	CloseWindow();

}

