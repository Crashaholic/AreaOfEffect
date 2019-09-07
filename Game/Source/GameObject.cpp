#include "gpch.h"

#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue)
	, scale(1, 1, 1)
	, active(false)
	, mass(1.f)
	, hookingClass(typeid(int))
{
}

GameObject::~GameObject()
{
}