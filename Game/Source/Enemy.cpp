#include "gpch.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
{
}

void Enemy::Init(GameObject* go)
{
	GO = go;
	GO->hookingClass = std::type_index(typeid(Enemy));
}

void Enemy::Update(double dt)
{
}
