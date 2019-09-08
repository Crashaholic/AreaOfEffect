#include "gpch.h"
#include "Entity.h"

Entity::~Entity()
{
	while (Statuses.size() > 0)
	{
		Statuses.pop_back();
	}
}

void Entity::Update(double dt)
{
}

void Entity::CheckStatuses()
{
	for (size_t i = 0; i < Statuses.size(); ++i)
	{
		if (Statuses[i].CheckExpired())
		{
			Statuses.erase(Statuses.begin() + i);
		}
	}
}

void Entity::AddStatusEffect(Damage dmg, double duration)
{

}

void Entity::TakeDamage(Damage dmg)
{
	health -= dmg.Total() - resistance.Total();
}

