#pragma once
#include "../gpch.h"
#include "../Scene.h"
#include "SingletonTemplate.h"
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

struct SceneNotRegistered : public std::exception
{
	const char* what() const throw ()
	{
		return "Scene is not registered";
	}
};

class SceneHandler : public Singleton<SceneHandler>
{
public:

	template <typename T>
	std::shared_ptr<Scene> GetScene()
	{
		if (SceneStorage.find(std::type_index(typeid(T))) != SceneStorage.end())
			return SceneStorage[std::type_index(typeid(T))];
		return nullptr;
	}

	template <typename T>
	inline bool AddScene()
	{
		if (GetScene<T>())
			return false;
		SceneStorage[std::type_index(typeid(T))] = std::make_shared<T>();
		return true;
	}

	template <typename T>
	void ChangeScene()
	{
		if (GetScene<T>() != nullptr)
		{
			if (ActiveScene == nullptr)
			{
				ActiveScene = SceneStorage[std::type_index(typeid(T))];
				ActiveScene->Init();
				return;
			}
			ActiveScene->Exit();
			ActiveScene = SceneStorage[std::type_index(typeid(T))];
			ActiveScene->Init();
			return;
		}
	}

	inline void RunScene(double dt)
	{
		ActiveScene->Update(dt);
		ActiveScene->Render();
	}

	inline std::shared_ptr<Scene> GetActiveScene()
	{
		return ActiveScene;
	}

	inline void CloseScene()
	{
		ActiveScene->Exit();
	}

	template <typename T>
	inline void StartUpScene()
	{
		if (GetScene<T>() != nullptr)
		{
			ActiveScene = SceneStorage[std::type_index(typeid(T))];
			ActiveScene->Init();
		}
		else
		{
			throw SceneNotRegistered();
		}
	}

	SceneHandler() {};
	//TODO: DESTRUCTOR
	~SceneHandler()
	{

	}

protected:
	std::shared_ptr<Scene> ActiveScene;
	std::unordered_map<std::type_index, std::shared_ptr<Scene>> SceneStorage;
};

