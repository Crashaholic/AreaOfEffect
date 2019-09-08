#pragma once
#include "gpch.h"

#include "Entity.h"

struct Enemy : public Entity
{
	Enemy();
	virtual void Init(GameObject* go) override;
	virtual void Update(double dt) override;
	virtual void Render() override;
};

