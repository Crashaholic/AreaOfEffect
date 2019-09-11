#pragma once

#include "gpch.h"

#include "CTimer.h"
#include "Damage.h"

struct StatusEffect
{
	StatusEffect(Damage d, double duration) { this->duration = duration; this->damage = d; timer.StartTimer(); };
	Timer timer;
	double duration;
	bool CheckExpired() { return (timer.Lap() - duration) <= 0.00f; };
	Damage damage;
};