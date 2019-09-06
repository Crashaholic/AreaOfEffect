#pragma once
#include <gpch.h>

struct Damage
{
	Damage(float p, float f, float c, float l);
	Damage(const Damage& d);
	Damage();
	~Damage();

	Damage Percent(float Percent);
	float Total();

	float Phys;
	float Fire;
	float Cold;
	float Light;

	Damage  operator+  (const Damage& d) const;
	Damage  operator-  (const Damage& d) const;
	Damage  operator*  (const Damage& d) const;
	Damage  operator/  (const Damage& d) const;

	Damage  operator*  (const int&    i) const;
	Damage  operator*  (const float&  f) const;
	Damage  operator*  (const double& d) const;

	Damage& operator+= (const Damage& d);
	Damage& operator-= (const Damage& d);
	Damage& operator*= (const Damage& d);
	Damage& operator/= (const Damage& d);

	Damage& operator=  (const Damage& d);
	Damage  operator-  (     void      ) const; // Unary negation

	bool    operator== (const Damage& d) const;
	bool    operator!= (const Damage& d) const;

	bool    operator>  (const Damage& d) const;
	bool    operator<  (const Damage& d) const;

	friend std::ostream& operator<< (std::ostream& os, Damage& d);
};

