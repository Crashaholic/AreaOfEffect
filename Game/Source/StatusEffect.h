#pragma once

#include "gpch.h"

#include "Damage.h"

struct StatusEffect
{
	StatusEffect(Damage d, double duration) { this->duration = duration; this->damage = d; timer.startTimer(); };
	StopWatch timer;
	double duration;
	bool CheckExpired() { return (timer.getElapsedTime() - duration) <= 0.00f; };
	Damage damage;
};