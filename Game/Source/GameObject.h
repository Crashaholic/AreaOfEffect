#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include <typeindex>

struct GameObject
{
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;

	std::type_index hookingClass;

	GameObject();
	~GameObject();

	GLuint textureID;
};

#endif