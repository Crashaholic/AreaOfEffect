#include <gpch.h>
#include "GOManager.h"

GameObject* GOManager::FetchGO()
{
	for (size_t i = 0; i < GOContainer.size(); ++i)
	{
		if (!GOContainer[i]->active)
		{
			return GOContainer[i];
		}
	}
}

