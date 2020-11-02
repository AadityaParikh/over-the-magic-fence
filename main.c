#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "spell.h"
#include "person.h"

#define PLAYER 0

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
	Vector3 screenDir; // normalized vector of direction screen is pointing in

	float temp = 0.0f;
	int frame = 0;

	Person people[10] = {0};
	int numPeople = (sizeof(people)/sizeof(people[0])); // for ease of use
	Person sortedPeople[numPeople]; // player not included

	people[PLAYER].spells[0].name = "fireball";
	people[PLAYER].spells[0].speed = 0.5f;
	people[PLAYER].spells[0].coolDown = 60; // 1 sec
	people[PLAYER].numSpells = 1;
	people[PLAYER].position.y = 1;


	for(int i = 0;i<people[PLAYER].numSpells;i++) { // loading sprite textures
		for(int j = 0;j<4;j++) {
			char filename[100];
			sprintf(filename,"Art/Sprites/spells/%s/sprite_%d.png",people[PLAYER].spells[i].name,j);
			people[PLAYER].spells[i].sprites[j] = LoadTexture(filename);
		}
	}

	people[1].sprites[0] = LoadTexture("Art/Sprites/Player movement/Movement frames/Movement 1/sprite_0.png");
	people[1].position.y = 1;
	people[1].position.x = 1;
	people[1].momentum = (Vector3){0};
	people[2].sprites[0] = LoadTexture("Art/Sprites/Player movement/Movement frames/Movement 1/sprite_0.png");
	people[2].position.y = 1;
	people[2].momentum = (Vector3){0};


	char debugText[15][50] = {0};
	while(!WindowShouldClose()) {

		UpdateCamera(&camera);
		frame++;
		screenDir = subVector3(people[PLAYER].target,people[PLAYER].position);
		screenDir = normVector3(screenDir);

		people[PLAYER].position.x = camera.position.x; // player specific
		camera.position.y = people[PLAYER].position.y;
		people[PLAYER].position.z = camera.position.z;
		people[PLAYER].target = camera.target;
		people[PLAYER].direction = screenDir;

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&people[PLAYER].spellCooldown==0&&people[PLAYER].spellActive==0) { // spell init
			people[PLAYER].spells[0].pos = people[PLAYER].position;
			people[PLAYER].spells[0].init = people[PLAYER].position;
			people[PLAYER].spells[0].dir = screenDir;

			people[PLAYER].spellCooldown =  people[PLAYER].spells[people[PLAYER].curSpell].coolDown;
			people[PLAYER].spellActive = 1;
		} // only done for player cause only player can click

		for(int j = 0;j<numPeople;j++) {
			for(int i = 0;i<people[j].numSpells;i++) { // spell updating
				people[j].spells[i].pos = addVector3(people[j].spells[i].pos,scaleVector3(people[j].spells[i].dir,people[j].spells[i].speed));
				people[j].spells[i].len = magVector3(subVector3(people[j].spells[i].pos,people[j].spells[i].init));

				if(	(i == people[j].curSpell && people[j].spellActive) &&
					(people[j].spells[i].pos.y<=0.1f || // under the ground
					people[j].spells[i].pos.x>=100.0f || // out of bounds x
					people[j].spells[i].pos.y>=100.0f || // out of bounds y
					people[j].spells[i].pos.z>=100.0f || // out of bounds z
					people[j].spells[i].len > 50.0f)) { // too far away from spells' init

						people[j].spells[i].dir = zeroV3;
						people[j].spells[i].pos = (Vector3){0,1,0};
						people[j].spellActive = 0;
				}

				if(i != people[j].curSpell || !people[j].spellActive) {
					people[j].spells[i].pos = people[j].position;
					people[j].spells[i].pos.y = -1;
					people[j].spells[i].init = people[j].spells[i].pos;
				}
			}

			if(people[j].spellCooldown > 0) people[j].spellCooldown--;
			
		}

		sprintf(debugText[0],"%s : %d","FPS",GetFPS());
		sprintf(debugText[1],"%s : %s","Spell Active",people[PLAYER].spellActive?"Yes":"No");
		sprintf(debugText[2],"%s : %d","Cooldown",people[PLAYER].spellCooldown);
		sprintf(debugText[3],"%s : %f,%f,%f","Pos",people[PLAYER].position.x,people[PLAYER].position.y,people[PLAYER].position.z);
		sprintf(debugText[4],"%s : %f,%f,%f","Dir",people[PLAYER].direction.x,people[PLAYER].direction.y,people[PLAYER].direction.z);

		sprintf(debugText[5],"%s : %d","Current Spell Info",people[PLAYER].curSpell);
		sprintf(debugText[6],"\t%s : %s","Name",people[PLAYER].spells[people[PLAYER].curSpell].name);
		sprintf(debugText[7],"\t%s : %f,%f,%f","Pos",people[PLAYER].spells[people[PLAYER].curSpell].pos.x,people[PLAYER].spells[people[PLAYER].curSpell].pos.y,people[PLAYER].spells[people[PLAYER].curSpell].pos.z);
		sprintf(debugText[8],"\t%s : %f,%f,%f","Dir",people[PLAYER].spells[people[PLAYER].curSpell].dir.x,people[PLAYER].spells[people[PLAYER].curSpell].dir.y,people[PLAYER].spells[people[PLAYER].curSpell].dir.z);
		sprintf(debugText[9],"\t%s : %f,%f,%f","Init",people[PLAYER].spells[people[PLAYER].curSpell].init.x,people[PLAYER].spells[people[PLAYER].curSpell].init.y,people[PLAYER].spells[people[PLAYER].curSpell].init.z);
		sprintf(debugText[10],"\t%s : %d","MaxCooldown",people[PLAYER].spells[people[PLAYER].curSpell].coolDown);
		sprintf(debugText[11],"\t%s : %f","Speed",people[PLAYER].spells[people[PLAYER].curSpell].speed);
		sprintf(debugText[12],"\t%s : %f","Distance",people[PLAYER].spells[people[PLAYER].curSpell].len);

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(camera);
				
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},(Color){0,117,44,230});
				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);
				
				DrawBillboard(camera,people[PLAYER].spells[0].sprites[(frame/10)%4],people[PLAYER].spells[0].pos,1.0f,WHITE);

				DrawBillboard(camera,people[1].sprites[0],people[1].position,1.0f,WHITE);
				DrawBillboard(camera,people[2].sprites[0],people[2].position,1.0f,WHITE);


				
				
			EndMode3D();

			for(int i = 0;i<(sizeof(debugText)/sizeof(debugText[0]));i++) {
				DrawText(debugText[i],10,10+(i*20),20,BLACK);
			}

		EndDrawing();
	}
	CloseWindow();

}

