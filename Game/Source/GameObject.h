#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include <typeindex>
#include "Sprite.h"

struct GameObject
{
	Vector3 pos;
	Vector3 vel;
	float rotation;
	float scale;
	bool active;
	float mass;

	std::map<std::string, Sprite> sprites;
	std::type_index hookingClass;

	GameObject();
	~GameObject();

	Sprite activeSprite;
	float transparency;
};

#endif