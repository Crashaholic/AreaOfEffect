#pragma once

#include "Damage.h"
#include "Resistances.h"

Damage operator- (const Damage& d, const Resistances& r)
{
	return Damage
	( 
		(d.Phys  * (1.f - r.Phys  / 100.f)),
		(d.Fire  * (1.f - r.Fire  / 100.f)),
		(d.Cold  * (1.f - r.Cold  / 100.f)),
		(d.Light * (1.f - r.Light / 100.f))
	);
}


/*
	d = (10, 0, 0, 0)
	r = (10, 0, 0, 0) REMEMBER: R IS IN PERCENTAGES

	10 * (1 - 10 / 100) = 9
*/

