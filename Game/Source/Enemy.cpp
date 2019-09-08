#include "Entity.h"
#include "gpch.h"
#include "Enemy.h"

Enemy::Enemy()
{
}

void Enemy::Init(GameObject* go)
{
	GO->hookingClass = std::type_index(typeid(Enemy));
}

void Enemy::Update(double dt)
{
}

void Enemy::Render()
{
}
