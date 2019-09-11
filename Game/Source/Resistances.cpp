#include "gpch.h"
#include "Resistances.h"

Resistances::Resistances()
{
}

Resistances::Resistances(float p, float f, float c, float l)
{
	Phys = p;
	Fire = f;
	Cold = c;
	Light = l;
}

Resistances::~Resistances()
{
}

float Resistances::Total()
{
	return Phys + Fire + Cold + Light;
}
