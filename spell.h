#ifndef SPELL_H
#define SPELL_H
#include <math.h>

typedef struct Spell {
    Vector3 pos;
    Vector3 dir;
    Vector3 init;
    float speed;
    Texture2D spriteSheet;
	int curSprite;
    char* name;
    int coolDown;
    float len;
} Spell;

#endif
