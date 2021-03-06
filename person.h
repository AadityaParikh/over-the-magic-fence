#ifndef PERSON_H
#define PERSON_H

#include <raylib.h>

#define DIR_NORTH	0; // north being the position that faces the player
#define DIR_NWEST	1; // player-east, person-west
#define DIR_WEST	2;
#define DIR_SWEST	3;
#define DIR_SOUTH	4;
#define DIR_SEAST	5;
#define DIR_EAST	6;
#define DIR_NEAST	7;

typedef struct Person {
	char name[50];
	int spellCooldown; // cooldown UNTIL spell is cast
	int curSpell;
	bool spellActive;
	Spell spells[15];
	int numSpells;
	Vector3 position;
	Vector3 direction;
	// below vars not needed for player
	int cDir;
	int deathFrames; // frames since death
	Rectangle curSprite;
	Vector3 target;
	Vector3 momentum;
	Texture2D spriteSheet;
	int spriteW;
	int spriteH;
	float lenFromPlayer;
} Person;



#endif
