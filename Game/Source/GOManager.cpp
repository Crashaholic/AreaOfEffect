#include "gpch.h"
#include "GOManager.h"

GOManager::~GOManager() 
{
	for (size_t i = 0; i < GOContainer.size(); ++i)
	{
		if (GOContainer[i])
		{
			if (GOContainer[i]->active)
			{
				delete GOContainer[i];
				GOContainer.erase(GOContainer.begin() + i);
			}
		}
	}
}

void GOManager::Init()
{
	for (int i = 0; i < 10; i++)
	{
		GOContainer.push_back(new GameObject);
	}
}

GameObject* GOManager::FetchGO()
{
	for (size_t i = 0; i < GOContainer.size(); ++i)
	{
		if (!GOContainer[i]->active)
		{
			return GOContainer[i];
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GOContainer.push_back(new GameObject);
	}
	return GOContainer.back();
}



