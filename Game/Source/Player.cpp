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
		GO->vel.x += speed * dt;
	else
		GO->vel.x -= speed * dt;
	std::cout  << GO->vel.x << '\n';
	GO->vel.x = Math::Clamp(GO->vel.x, -maxSpeed, maxSpeed);
}

void Player::MoveY_KB(bool dirY, double dt)
{
	if (dirY)
		GO->vel.y += speed * dt;
	else
		GO->vel.y -= speed * dt;
	GO->vel.y = Math::Clamp(GO->vel.y, -maxSpeed, maxSpeed);
}

void Player::MoveX_Pad(float axis, double dt)
{
	float x;
	if (axis > 0)
	{
		x = (axis - -0.8f) / (0.8f - -0.8f);
	}
	else
	{
		x = -(axis - 0.8f) / (-0.8f - 0.8f);
	}
	GO->vel.x += x * speed * dt;
	std::cout << "x:\t" << x << "\t\t\t" << GO->vel.x << '\n';
	GO->vel.x = Math::Clamp(GO->vel.x, -maxSpeed, maxSpeed);
}

void Player::MoveY_Pad(float axis, double dt)
{
	float y;
	if (axis > 0)
	{
		y = (axis - -0.8f) / (0.8f - -0.8f);
	}
	else
	{
		y = -(axis - 0.8f) / (-0.8f - 0.8f);
	}
	GO->vel.y -= y * speed * dt;
	std::cout << "y:\t" << y << "\t\t\t" << GO->vel.y << '\n';
	GO->vel.y = Math::Clamp(GO->vel.y, -maxSpeed, maxSpeed);
}
