#pragma once
#include "gpch.h"
#include "GameObject.h"

#include "StatusEffect.h"
#include "Resistances.h"

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
	virtual void Render() = 0;
	void CheckStatuses();
	void AddStatusEffect(Damage dmg, double duration);

	void TakeDamage(Damage dmg);


	float health;
	float speed;
	float maxSpeed;

};

