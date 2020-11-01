#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "spell.h"
#include "person.h"

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
	Mesh ringMesh = LoadModel("Art/Models/ring.obj");

	Vector3 zeroV3 = {0.0f,0.0f,0.0f};
	Vector3 screenDir = zeroV3; // normalized vector of direction screen is pointing in

	Ray mouse = {0};
	
	float temp = 0.0f;
	int frame = 0;

	Person people[10];

	people[0].spells[0].name = "fireball";
	people[0].spells[0].speed = 0.5f;
	people[0].spells[0].coolDown = 60; // 1 sec
	people[0].numSpells = 15;


	for(int j = 0;j<people[0].numSpells;j++) { // loading sprite textures
		for(int i = 0;i<4;i++) {
			char filename[100];
			sprintf(filename,"Art/Sprites/%s/sprite_%d.png",people[0].spells[j].name,i);
			people[0].spells[j].sprites[i] = LoadTexture(filename);
		}
	}

	Texture2D enemy = LoadTexture("Art/Sprites/Player movement/Movement frames/Movement 1/sprite_0.png");


	char debugText[15][50] = {0};
	while(!WindowShouldClose()) {

		UpdateCamera(&camera);
		frame++;
		people[0].position = camera.position; // player specific
		people[0].target = camera.target;

		if(people[0].spellCooldown > 0) people[0].spellCooldown--;

		screenDir = subVector3(people[0].target,people[0].position);
		screenDir = normVector3(screenDir);
		people[0].direction = screenDir;

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&people[0].spellCooldown==0&&people[0].spellActive==0) { // spell init
			people[0].spells[0].pos = people[0].position;
			people[0].spells[0].init = people[0].position;
			people[0].spells[0].dir = screenDir;

			people[0].spellCooldown =  people[0].spells[people[0].curSpell].coolDown;
			people[0].spellActive = 1;
		}

		for(int i = 0;i<people[0].numSpells;i++) { // spell updating
			people[0].spells[i].pos = addVector3(people[0].spells[i].pos,scaleVector3(people[0].spells[i].dir,people[0].spells[i].speed));
			people[0].spells[i].len = magVector3(subVector3(people[0].spells[i].pos,people[0].spells[i].init));

			if(	(i == people[0].curSpell && people[0].spellActive) &&
				(people[0].spells[i].pos.y<=0.1f || // under the ground
				people[0].spells[i].pos.x>=100.0f || // out of bounds x
				people[0].spells[i].pos.y>=100.0f || // out of bounds y
				people[0].spells[i].pos.z>=100.0f || // out of bounds z
				people[0].spells[i].len > 50.0f)) { // too far away from spells' init

					people[0].spells[i].dir = zeroV3;
					people[0].spells[i].pos = (Vector3){0,1,0};
					people[0].spellActive = 0;
			}

			if(i != people[0].curSpell || !people[0].spellActive) {
				people[0].spells[i].pos = people[0].position;
				people[0].spells[i].pos.y = -1;
				people[0].spells[i].init = people[0].spells[i].pos;
			}
		}

		sprintf(debugText[0],"%s : %d","FPS",GetFPS());
		sprintf(debugText[1],"%s : %s","Spell Active",people[0].spellActive?"Yes":"No");
		sprintf(debugText[2],"%s : %d","Cooldown",people[0].spellCooldown);
		sprintf(debugText[3],"%s : %f,%f,%f","Pos",people[0].position.x,people[0].position.y,people[0].position.z);
		sprintf(debugText[4],"%s : %f,%f,%f","Dir",people[0].direction.x,people[0].direction.y,people[0].direction.z);

		sprintf(debugText[5],"%s : %d","Current Spell Info",people[0].curSpell);
		sprintf(debugText[6],"\t%s : %s","Name",people[0].spells[people[0].curSpell].name);
		sprintf(debugText[7],"\t%s : %f,%f,%f","Pos",people[0].spells[people[0].curSpell].pos.x,people[0].spells[people[0].curSpell].pos.y,people[0].spells[people[0].curSpell].pos.z);
		sprintf(debugText[8],"\t%s : %f,%f,%f","Dir",people[0].spells[people[0].curSpell].dir.x,people[0].spells[people[0].curSpell].dir.y,people[0].spells[people[0].curSpell].dir.z);
		sprintf(debugText[9],"\t%s : %f,%f,%f","Init",people[0].spells[people[0].curSpell].init.x,people[0].spells[people[0].curSpell].init.y,people[0].spells[people[0].curSpell].init.z);
		sprintf(debugText[10],"\t%s : %d","MaxCooldown",people[0].spells[people[0].curSpell].coolDown);
		sprintf(debugText[11],"\t%s : %f","Speed",people[0].spells[people[0].curSpell].speed);
		sprintf(debugText[12],"\t%s : %f","Distance",people[0].spells[people[0].curSpell].len);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);
				
				
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},DARKGREEN);
				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);
				
				DrawBillboard(camera,enemy,(Vector3){0,1,0},1.0f,WHITE);
				DrawBillboard(camera,enemy,(Vector3){1,1,1},1.0f,WHITE);

				DrawBillboard(camera,people[0].spells[0].sprites[(frame/10)%4],people[0].spells[0].pos,1.0f,WHITE);

				
				
			EndMode3D();

			for(int i = 0;i<(sizeof(debugText)/sizeof(debugText[0]));i++) {
				DrawText(debugText[i],10,10+(i*20),20,BLACK);
			}

		EndDrawing();
	}
	CloseWindow();

}

