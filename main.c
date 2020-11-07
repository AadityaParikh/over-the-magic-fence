#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "spell.h"
#include "person.h"
#include "func.h"


#define PLAYER 0
#define PSPRITEW 30
#define PSPRITEH 50


int main(int argc, char** argv) {
	int screenWidth = GetMonitorWidth(0);
	int screenHeight = GetMonitorHeight(0);
	InitWindow(screenWidth,screenHeight,"Over The Magic Fence");
	SetTargetFPS(60);

	Camera3D camera = {0}; 
    camera.position = (Vector3){ 1.0f, 1.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; 
    camera.fovy = 90.0f;
    camera.type = CAMERA_PERSPECTIVE;
	SetCameraMode(camera,CAMERA_FIRST_PERSON);

	Camera3D rendered = camera; // actual camera that will be rendered

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
	camera.position.y = 1; // don't be a dumbass like me, edit camera pos instead of player pos
	// player pos is for math use only
	Vector2 dodgeDir = {0};
	
	
	Vector3 inter = camera.position; // intermediary between camera


	for(int i = 0;i<people[PLAYER].numSpells;i++) { // loading sprite textures
		for(int j = 0;j<4;j++) {
			char filename[100];
			sprintf(filename,"Art/Sprites/spells/%s/sprite_%d.png",people[PLAYER].spells[i].name,j);
			people[PLAYER].spells[i].sprites[j] = LoadTexture(filename);
		}
	}

	people[1].spriteSheet = LoadTexture("Art/Sprites/people/betty/spritesheet.png");
	people[1].position.y = 0.84f;
	people[1].position.x = 1;
	people[1].curSprite.width = PSPRITEW;
	people[1].curSprite.height = PSPRITEH;
	people[1].momentum = (Vector3){0};
	people[2].spriteSheet = LoadTexture("Art/Sprites/people/betty/spritesheet.png");
	people[2].position.y = 0.84f;
	people[2].curSprite.width = PSPRITEW;
	people[2].curSprite.height = PSPRITEH;
	people[2].momentum = (Vector3){0};


	screenWidth = GetMonitorWidth(0);
	screenHeight = GetMonitorHeight(0);
	char debugText[20][50] = {0};
	while(!WindowShouldClose()) {


		UpdateCamera(&camera);
		rendered.position = addVector3(scaleVector3(camera.position,0.2),scaleVector3(rendered.position,0.8));
		rendered.target = addVector3(scaleVector3(camera.target,0.2),scaleVector3(rendered.target,0.8));
		frame++;

		people[PLAYER].position = camera.position;
		people[PLAYER].target = camera.target;
		screenDir = subVector3(people[PLAYER].target,people[PLAYER].position);
		screenDir = normVector3(screenDir);
		people[PLAYER].direction = screenDir;

		dodgeDir.x = people[PLAYER].direction.x;
		dodgeDir.y = people[PLAYER].direction.z;

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&people[PLAYER].spellCooldown==0&&people[PLAYER].spellActive==0) { // spell init
			people[PLAYER].spells[0].pos = people[PLAYER].position;
			people[PLAYER].spells[0].init = people[PLAYER].position;
			people[PLAYER].spells[0].dir = screenDir;

			people[PLAYER].spellCooldown =  people[PLAYER].spells[people[PLAYER].curSpell].coolDown;
			people[PLAYER].spellActive = 1;
		} 
		switch(GetKeyPressed()) {
			case KEY_A :
				
			case KEY_SPACE :
				camera.position.x += people[PLAYER].direction.x*5;
				camera.position.z += people[PLAYER].direction.z*5;
				break;
			default :
				break;
		}

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

			/*if(people[j].direction.x == 0 || people[j].direction.y == 0 || people[j].direction.z == 0) { // can't compare Vec3 directly it seems
				people[j].curSprite.x = 0;
				people[j].curSprite.y = 0;
			}*/
			
			people[j].curSprite.x = ((frame/10)%6)*PSPRITEW;
			people[j].curSprite.y = people[j].cDir*PSPRITEH;
			
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
		sprintf(debugText[13],"%s : %d","Key Pressed",GetKeyPressed());

		BeginDrawing();
			ClearBackground(SKYBLUE);

			BeginMode3D(rendered);
				
				DrawPlane((Vector3){0.0f,-0.0001f,0.0f},(Vector2){100.0f,100.0f},(Color){0,117,44,230});
				DrawModel(ring,(Vector3){0.0f,0.0f,0.0f},1.0f,GRAY);
				DrawModelWires(ring,zeroV3,1.0f,BLACK);
				DrawGizmo(zeroV3);
				
				DrawBillboard(rendered,people[PLAYER].spells[0].sprites[(frame/10)%4],people[PLAYER].spells[0].pos,1.0f,WHITE);

				DrawBillboardRec(rendered,people[1].spriteSheet,people[1].curSprite,people[1].position,1.0f,WHITE);
				DrawBillboardRec(rendered,people[2].spriteSheet,people[2].curSprite,people[2].position,1.0f,WHITE);


				
				
			EndMode3D();

			for(int i = 0;i<(sizeof(debugText)/sizeof(debugText[0]));i++) {
				DrawText(debugText[i],10,10+(i*20),20,BLACK);
			}

		EndDrawing();
	}
	CloseWindow();

}

