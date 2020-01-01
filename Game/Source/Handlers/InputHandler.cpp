#include "gpch.h"
#include "InputHandler.h"
#include "../Application.h"

KbKey::KbKey()
	: pressed(false)
	, lastState(false)
	, associatedKey(0)
{
}

KbKey::KbKey(unsigned short k)
	: pressed(false)
	, lastState(false)
	, associatedKey(k)
{
}

KbKey::~KbKey()
{
}

bool KbKey::IsPressed()
{
	if (glfwGetWindowAttrib(Application::GetInstance().GetWindow(), GLFW_FOCUSED))
	{
		pressed = ((GetAsyncKeyState(associatedKey) & 0x8001) != 0);
	}

	if (pressed == false)
	{
		lastState = false;
	}
	if (lastState == true)
	{
		return false;
	}
	if (pressed == true)
	{
		lastState = true;
		return true;
	}
	return false;
}

bool KbKey::IsPressed(bool JustDoIt)
{
	if (glfwGetWindowAttrib(Application::GetInstance().GetWindow(), GLFW_FOCUSED))
	{
		return ((GetAsyncKeyState(associatedKey) & 0x8001) != 0);
	}
	return false;
}

void KbKey::SetAssociatedKey(unsigned short k)
{
	associatedKey = k;
}

bool InputHandler::GetKeyPressed(unsigned short k)
{
	for (std::pair<unsigned short, KbKey*> x : KeyStorage)
	{
		if (k == x.first)
		{
			return x.second->IsPressed();
		}
	}
	return false;
}

bool InputHandler::GetKeyPressed(unsigned short k, bool JustDoIt)
{
	for (std::pair<unsigned short, KbKey*> x : KeyStorage)
	{
		if (k == x.first)
		{
			return x.second->IsPressed(JustDoIt);
		}
	}
	return false;
}
