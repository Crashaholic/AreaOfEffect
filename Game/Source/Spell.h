#pragma once

#include "gpch.h"
#include "Damage.h"
#include "Entity.h"
#include "CTimer.h"

struct Spell : public Entity
{
	unsigned short baseRadius = 2;
	unsigned short radius;
	unsigned short delay;
	Damage dmg;
	Timer timer;

	void DropAt(vec3 position);
	virtual void Init(GameObject* go);
	virtual void Update(double dt);
	virtual void Render();
};

