#pragma once

#include "gpch.h"
#include "Entity.h"
#include "Spell.h"

#include "Camera.h"

#include <typeindex>
#include <typeinfo>

struct Player : public Entity
{
	Player() {}

	virtual void Init(GameObject* go);
	void InitCam(Camera* cam);
	std::vector<Spell> CurrentHand;
	Camera* cameraAttachment;
	void MoveX_KB(bool dirX, double dt); // 0 - left, 1 - right
	void MoveY_KB(bool dirY, double dt); // 0 - down, 1 - up

	void MoveX_Pad(float axis, double dt);
	void MoveY_Pad(float axis, double dt);

	virtual void Render() override {};
};
