#pragma once

#include "Entity.h"
#include "Spell.h"

struct Projectile : public Entity
{
	vec3 targetArea = {0,0,0};
	Spell spellToCastAtArea;
	virtual void Init(GameObject* go)
	{
		GO = go;
		GO->hookingClass = std::type_index(typeid(Projectile));
	}

	void AttachSpell(Spell s)
	{
		spellToCastAtArea = s;
	}

	virtual void Update(double dt)
	{
	}
};
