#pragma once
#include "gpch.h"
#include "GameObject.h"

#include "StatusEffect.h"
#include "Resistances.h"
#include "SpriteAnimation.h"

struct Entity
{
	GameObject* GO;

	Entity() : markedForDeletion(false) {};
	Entity(GameObject* GO) : markedForDeletion(false), health(1), maxHealth(1), speed(1), maxSpeed(1) { this->GO = GO; };
	~Entity();

	Resistances resistance;

	std::vector<StatusEffect> Statuses;
	virtual void Init(GameObject* go) = 0;
	virtual void Update(double dt);
	void CheckStatuses();
	void AddStatusEffect(Damage dmg, double duration);

	void TakeDamage(Damage dmg, double dt);

	float health;
	float maxHealth;
	float speed;
	float maxSpeed;

	bool markedForDeletion;

};

