#pragma once
#include <gpch.h>

#include "Damage.h"

struct StatusEffect
{
	StopWatch timer;
	double duration;
	Damage damage;
};

