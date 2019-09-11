#include "gpch.h"
#include "Player.h"

void Player::Init(GameObject * go)
{
	GO = go;
	(GO->hookingClass) = std::type_index(typeid(Player));
	speed = 100;
	maxSpeed = 20;
	health = 20;
	yeetSpeed = 50;
}

void Player::InitCam(Camera * cam)
{ 
	this->cameraAttachment = cam;
}

void Player::MoveX_KB(bool dirX, double dt)
{
	if (dirX)
		GO->vel.x += speed * (float)dt;
	else
		GO->vel.x -= speed * (float)dt;
	GO->vel.x = Math::Clamp(GO->vel.x, -maxSpeed, maxSpeed);
}

void Player::MoveY_KB(bool dirY, double dt)
{
	if (dirY)
		GO->vel.y += speed * (float)dt;
	else
		GO->vel.y -= speed * (float)dt;
	GO->vel.y = Math::Clamp(GO->vel.y, -maxSpeed, maxSpeed);
}

void Player::MoveX_Pad(float axis, double dt)
{
	float x;
	if (axis > 0)
		x = (axis - -0.8f) / (0.8f - -0.8f);
	else
		x = -(axis - 0.8f) / (-0.8f - 0.8f);
	GO->vel.x += x * speed * (float)dt;
	GO->vel.x = Math::Clamp(GO->vel.x, -maxSpeed, maxSpeed);
}

void Player::MoveY_Pad(float axis, double dt)
{
	float y;
	if (axis > 0)
		y = (axis - -0.8f) / (0.8f - -0.8f);
	else
		y = -(axis - 0.8f) / (-0.8f - 0.8f);
	GO->vel.y -= y * speed * (float)dt;
	GO->vel.y = Math::Clamp(GO->vel.y, -maxSpeed, maxSpeed);
}
