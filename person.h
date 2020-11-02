#ifndef PERSON_H
#define PERSON_H

#include "spell.h"
#include <raylib.h>

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

typedef enum {
	DIR_NORTH	= 0;
	DIR_NEAST	= 1;
	DIR_EAST	= 2;
	DIR_SEAST	= 3;
	DIR_SOUTH	= 4;
	DIR_SWEST	= 5;
	DIR_WEST	= 6;
	DIR_NWEST	= 7;
}

#endif
