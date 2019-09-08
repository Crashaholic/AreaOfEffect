#include "gpch.h"
#include "Resistances.h"

Resistances::Resistances()
{
}

Resistances::~Resistances()
{
}

float Resistances::Total()
{
	return Phys + Fire + Cold + Light;
}
