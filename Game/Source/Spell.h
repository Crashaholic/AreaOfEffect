#pragma once

#include "gpch.h"
#include "Damage.h"
#include "Entity.h"
#include "CTimer.h"

struct Spell : public Entity
{
	unsigned short baseRadius = 2;
	unsigned short radius;
	float delay;
	float duration;
	Damage dmg;
	Timer timer;

	void DropAt(vec3 position);
	void DamageNearby(Entity* ent, double dt);
	virtual void Init(GameObject* go);
	virtual void Update(double dt);
};

