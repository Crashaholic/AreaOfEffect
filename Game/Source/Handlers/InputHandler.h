#pragma once
#include "../gpch.h"
#include <functional>

class KbKey // slightly more unique name incase of conflicts
{
public:
	bool pressed;
	bool lastState;
	unsigned short associatedKey;

	KbKey();
	KbKey(unsigned short k);
	~KbKey();

	bool IsPressed();
	bool IsPressed(bool JustDoIt);
	void SetAssociatedKey(unsigned short k);
};

class InputHandler
{
public:
	std::map<unsigned short, KbKey*> KeyStorage;
	bool GetKeyPressed(unsigned short k);
	bool GetKeyPressed(unsigned short k, bool JustDoIt);
};