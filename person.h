#ifndef PERSON_H
#define PERSON_H

#include <raylib.h>

#define DIR_NORTH	0;
#define DIR_NEAST	1;
#define DIR_EAST	2;
#define DIR_SEAST	3;
#define DIR_SOUTH	4;
#define DIR_SWEST	5;
#define DIR_WEST	6;
#define DIR_NWEST	7;

typedef struct Person {
	char name[50];
	int spellCooldown;
	int curSpell;
	bool spellActive;
	Spell spells[15];
	int numSpells;
	Vector3 position;
	Vector3 direction;
	Vector3 target;
	Vector3 momentum;
	Texture2D sprites[6];
} Person;



#endif
