#include "gpch.h"
#include "Player.h"

void Player::Init(GameObject * go)
{
	GO = go;
	(GO->hookingClass) = std::type_index(typeid(Player));
	speed = 50;
	maxSpeed = 20;
}

void Player::InitCam(Camera * cam) 
{ 
	this->cameraAttachment = cam;
}

void Player::MoveX_KB(bool dirX, double dt)
{
	if (dirX)
	{
		GO->vel.x += speed * dt;
	}
	else
	{
		GO->vel.x -= speed * dt;
	}
	GO->vel.x = Math::Clamp(GO->vel.x, -maxSpeed, maxSpeed);
	cameraAttachment->position.x = GO->pos.x;
	cameraAttachment->target.x = GO->pos.x;
}

void Player::MoveY_KB(bool dirY, double dt)
{
	if (dirY)
	{
		GO->vel.y += speed * dt;
	}
	else
	{
		GO->vel.y -= speed * dt;
	}
	GO->vel.y = Math::Clamp(GO->vel.y, -maxSpeed, maxSpeed);
	cameraAttachment->position.y = GO->pos.y;
	cameraAttachment->target.y = GO->pos.y;
}

void Player::MoveX_Pad(float axis, double dt)
{
	GO->vel.x += axis * speed * dt;
	GO->vel.x = Math::Clamp(GO->vel.x, -maxSpeed, maxSpeed);
	cameraAttachment->position.x = GO->pos.x;
	cameraAttachment->target.x = GO->pos.x;
}

void Player::MoveY_Pad(float axis, double dt)
{
	GO->vel.y -= axis * speed * dt;
	GO->vel.y = Math::Clamp(GO->vel.y, -maxSpeed, maxSpeed);
	cameraAttachment->position.y = GO->pos.y;
	cameraAttachment->target.y = GO->pos.y;
}
