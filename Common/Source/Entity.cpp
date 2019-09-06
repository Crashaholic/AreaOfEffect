#include <gpch.h>
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

	}
}

void Entity::ApplyStatusEffect()
{
}
