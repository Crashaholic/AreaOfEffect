#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <gpch.h>

struct GameObject
{
	vec3 pos;
	vec3 vel;
	vec3 scale;
	bool active;
	float mass;

	GameObject();
	~GameObject();
};

#endif