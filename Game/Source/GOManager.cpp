#include "gpch.h"
#include "GOManager.h"

GOManager::~GOManager() 
{
	while (GOContainer.size() > 0)
	{
		GameObject* go = GOContainer.back();
		if (go)
			delete go;
		GOContainer.pop_back();
	}
}

void GOManager::Init()
{
	GOContainer.push_back(new GameObject);
}

GameObject* GOManager::FetchGO()
{
	for (size_t i = 0; i < GOContainer.size(); ++i)
	{
		if (!GOContainer[i]->active)
		{
			GOContainer[i]->active = true;
			return GOContainer[i];
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GOContainer.push_back(new GameObject());
	}

	return FetchGO();
}



