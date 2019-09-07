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

	template <class T>
	bool isHookedby(unsigned int i)
	{
		return (GOContainer[i]->hookingClass == std::type_index(typeid(T)));
	}

};

#define GOIsHookedOnByClass(i, ClassName) isHookedby<ClassName>(i)
