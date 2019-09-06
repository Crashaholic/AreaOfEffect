#pragma once

#include "GameObject.h"
#include "gpch.h"

struct GOManager
{
	GOManager() { Init(); }
	~GOManager();

	void Init();

	std::vector<GameObject*> GOContainer;
	GameObject* FetchGO();
};

