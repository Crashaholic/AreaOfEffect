#include "gpch.h"
#include "Player.h"

void Player::Init(GameObject * go)
{
	GO = go;
	(GO->hookingClass) = std::type_index(typeid(Player));
	speed = 5;
	maxSpeed = 10;
}

void Player::InitCam(Camera * cam) 
{ 
	this->cameraAttachment = cam;
}

void Player::MoveX_KB(bool dirX, double dt)
{
	if (dirX)
	{
		GO->pos.x += speed * dt;
	}
	else
	{
		GO->pos.x -= speed * dt;
	}
	cameraAttachment->position.x = GO->pos.x;
	cameraAttachment->target.x = GO->pos.x;
}

void Player::MoveY_KB(bool dirY, double dt)
{
	if (dirY)
	{
		GO->pos.y += speed * dt;
	}
	else
	{
		GO->pos.y -= speed * dt;
	}
	cameraAttachment->position.y = GO->pos.y;
	cameraAttachment->target.y = GO->pos.y;
}

void Player::MoveX_Pad(float axis, double dt)
{
	GO->pos.x += axis * speed * dt;
	cameraAttachment->position.x = GO->pos.x;
	cameraAttachment->target.x = GO->pos.x;
}

void Player::MoveY_Pad(float axis, double dt)
{
	GO->pos.y -= axis * speed * dt;
	cameraAttachment->position.y = GO->pos.y;
	cameraAttachment->target.y = GO->pos.y;
}
