#pragma once

#include <GameObject.h>
#include <gpch.h>

struct GOManager
{
	GOManager() {};
	~GOManager() {};

	void Init() {};

	std::vector<GameObject*> GOContainer;
	GameObject* FetchGO();
};

