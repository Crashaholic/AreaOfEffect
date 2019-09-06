#pragma once
#include <gpch.h>
#include "GameObject.h"

#include "StatusEffect.h"

struct Entity
{
	GameObject* GO;

	Entity() {};
	Entity(GameObject* GO) { this->GO = GO; };
	~Entity();

	std::vector<StatusEffect> Statuses;
	virtual void Update(double dt);
	void CheckStatuses();
	void ApplyStatusEffect();

};

