#include "gpch.h"
#include "Player.h"

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
	std::cout << GO->pos.x << '\n';
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

double RoundOff(double N, double n)
{
	int h;
	double l, a, b, c, d, e, i, j, m, f, g;
	b = N;
	c = floor(N);

	for (i = 0; b >= 1; ++i)
		b = b / 10;

	d = n - i;
	b = N;
	b = b * pow(10, d);
	e = b + 0.5;
	if ((float)e == (float)ceil(b)) {
		f = (ceil(b));
		h = f - 2;
		if (h % 2 != 0) {
			e = e - 1;
		}
	}
	j = floor(e);
	m = pow(10, d);
	j = j / m;
	return j;
}

void Player::MoveX_Pad(float axis, double dt)
{
	float temp = (float)RoundOff((double)axis, 1);
	if (temp != 0.000f)
		std::cout << "LEFT STICK X: " << temp << ", ";
	GO->pos.x += temp * speed * dt;
	cameraAttachment->position.x = GO->pos.x;
	cameraAttachment->target.x = GO->pos.x;
}

void Player::MoveY_Pad(float axis, double dt)
{
	float temp = (float)RoundOff((double)axis, 1);
	if (temp != 0.000f)
		std::cout << "LEFT STICK Y: " << temp << '\n';
	GO->pos.y -= temp * speed * dt;
	cameraAttachment->position.y = GO->pos.y;
	cameraAttachment->target.y = GO->pos.y;
}
