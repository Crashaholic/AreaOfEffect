#include "gpch.h"

#include "GameObject.h"

GameObject::GameObject() 
	: scale(1)
	, active(false)
	, mass(1.f)
	, hookingClass(typeid(int))
	, transparency(0)
{
}

GameObject::~GameObject()
{
}