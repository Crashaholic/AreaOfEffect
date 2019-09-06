#include "gpch.h"
#include "Damage.h"

// Overloaded constructor to define a Damage type quickly
Damage::Damage(float p, float f, float c, float l)
	: Phys(p), Fire(f), Cold(c), Light(l)
{

}

Damage::Damage(const Damage & d) :
  Phys(d.Phys)
, Fire(d.Fire)
, Cold(d.Cold)
, Light(d.Light)
{
}

Damage::Damage() : Phys(0), Fire(0), Cold(0), Light(0)
{
}

Damage::~Damage()
{
}

Damage Damage::Percent(float percent)
{
	return Damage(
		this->Phys * (percent / 100),
		this->Fire * (percent / 100),
		this->Cold * (percent / 100),
		this->Light* (percent / 100)
	);
}

float Damage::Total()
{
	return this->Phys + this->Fire + this->Cold + this->Light;
}

bool IsEqual(float a, float b)
{
	return a - b <= 0.00001f && b - a <= 0.00001f;
}

Damage Damage::operator+(const Damage& d) const
{
	return Damage(
		this->Phys  + d.Phys,
		this->Fire  + d.Fire,
		this->Cold  + d.Cold,
		this->Light + d.Light
	);
}

Damage Damage::operator-(const Damage& d) const
{
	return Damage(
		this->Phys  - d.Phys,
		this->Fire  - d.Fire,
		this->Cold  - d.Cold,
		this->Light - d.Light
	);
}

Damage Damage::operator*(const Damage& d) const
{
	return Damage(
		this->Phys  * d.Phys,
		this->Fire  * d.Fire,
		this->Cold  * d.Cold,
		this->Light * d.Light
	);
}

Damage Damage::operator/(const Damage& d) const
{
	return Damage(
		this->Phys  / d.Phys,
		this->Fire  / d.Fire,
		this->Cold  / d.Cold,
		this->Light / d.Light
	);
}

Damage Damage::operator*(const int & i) const
{
	return Damage(
		this->Phys	* i,
		this->Fire	* i,
		this->Cold	* i,
		this->Light	* i
	);
}

Damage Damage::operator*(const float & f) const
{
	return Damage(
		this->Phys	* f,
		this->Fire	* f,
		this->Cold	* f,
		this->Light	* f
	);
}

Damage Damage::operator*(const double & d) const
{
	return Damage(
		(float)((double)this->Phys	* d),
		(float)((double)this->Fire	* d),
		(float)((double)this->Cold	* d),
		(float)((double)this->Light	* d)
	);
}

Damage& Damage::operator+=(const Damage & d)
{
	this->Phys  += d.Phys;
	this->Fire  += d.Fire;
	this->Cold  += d.Cold;
	this->Light += d.Light;
	return *this;
}

Damage& Damage::operator-=(const Damage & d)
{
	this->Phys  -= d.Phys;
	this->Fire  -= d.Fire;
	this->Cold  -= d.Cold;
	this->Light -= d.Light;
	return *this;
}

Damage& Damage::operator*=(const Damage & d)
{
	this->Phys  *= d.Phys;
	this->Fire  *= d.Fire;
	this->Cold  *= d.Cold;
	this->Light *= d.Light;
	return *this;
}

Damage& Damage::operator/=(const Damage & d)
{
	this->Phys  /= d.Phys;
	this->Fire  /= d.Fire;
	this->Cold  /= d.Cold;
	this->Light /= d.Light;
	return *this;
}

Damage& Damage::operator=(const Damage & d)
{
	this->Phys  = d.Phys;
	this->Fire  = d.Fire;
	this->Cold  = d.Cold;
	this->Light = d.Light;
	return *this;
}

Damage Damage::operator-(void) const
{
	return Damage(
		-this->Phys,
		-this->Fire,
		-this->Cold,
		-this->Light
	);
}

bool Damage::operator==(const Damage & d) const
{
	return
	IsEqual(this->Phys , d.Phys ) &&
	IsEqual(this->Fire , d.Fire ) &&
	IsEqual(this->Cold , d.Cold ) &&
	IsEqual(this->Light, d.Light);
}

bool Damage::operator!=(const Damage & d) const
{
	return
	!IsEqual(this->Phys , d.Phys ) ||
	!IsEqual(this->Fire , d.Fire ) ||
	!IsEqual(this->Cold , d.Cold ) ||
	!IsEqual(this->Light, d.Light);
}

bool Damage::operator>(const Damage & d) const
{
	return 
		this->Phys  > d.Phys  &&
		this->Fire  > d.Fire  &&
		this->Cold  > d.Cold  &&
		this->Light > d.Light;
}

bool Damage::operator<(const Damage & d) const
{
	return 
		this->Phys  < d.Phys  &&
		this->Fire  < d.Fire  &&
		this->Cold  < d.Cold  &&
		this->Light < d.Light;
}

std::ostream & operator<<(std::ostream& os, Damage & d)
{
	std::streamsize ss = os.precision();
	os.precision(0);
	os <<
		"P: " << d.Phys  << " | " <<
		"F: " << d.Fire  << " | " <<
		"C: " << d.Cold  << " | " <<
		"L: " << d.Light;
	os.precision(ss);
	return os;
}
