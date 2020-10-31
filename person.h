#ifndef PERSON_H
#define PERSON_H

#include "spell.h"
#include <raylib.h>

typedef struct Person {
	int spellCooldown;
	int curSpell;
	bool spellActive;
	Spell spells[15];
	int numSpells;
	Vector3 position;
	Vector3 direction;
	Vector3 target;
	Texture2D sprites;
} Person;

#endif
