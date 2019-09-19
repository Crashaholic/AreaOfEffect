#include "gpch.h"
#include "Spell.h"

void Spell::Init(GameObject* go)
{
	GO = go;
	GO->hookingClass = std::type_index(typeid(Spell));
}

void Spell::Update(double dt)
{
}


void Spell::DropAt(vec3 pos)
{
	GO->pos = pos;
}

void Spell::DamageNearby(Entity * ent, double dt)
{
	if ((ent->GO->pos - this->GO->pos).Length() <= radius)
	{
		ent->TakeDamage(dmg, dt);
	}
}
