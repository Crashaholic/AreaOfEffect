#pragma once
struct Resistances
{
	Resistances();
	Resistances(float, float, float, float);
	~Resistances();

	float Total();

	float Phys;
	float Fire;
	float Cold;
	float Light;
};

