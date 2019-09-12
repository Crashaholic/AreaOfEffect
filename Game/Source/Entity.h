#pragma once
#include "gpch.h"
#include "GameObject.h"

#include "StatusEffect.h"
#include "Resistances.h"
#include "SpriteAnimation.h"

struct Entity
{
	GameObject* GO;

	Entity() {};
	Entity(GameObject* GO) { this->GO = GO; };
	~Entity();

	Resistances resistance;

	std::vector<StatusEffect> Statuses;
	virtual void Init(GameObject* go) = 0;
	virtual void Update(double dt);
	void CheckStatuses();
	void AddStatusEffect(Damage dmg, double duration);

	void TakeDamage(Damage dmg);

	float health;
	float maxHealth;
	float speed;
	float maxSpeed;

};

