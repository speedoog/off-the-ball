/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeVector helper class implementation
*/


#include "hgevector.h"

hgeVector hgeVector::ZERO(0.0f, 0.0f);
hgeVector hgeVector::UNIT_X(1.0f, 0.0f);
hgeVector hgeVector::UNIT_Y(0.0f, 1.0f);

float InvSqrt(float x)
{
		union
        {
          int intPart;
          float floatPart;
        } convertor;

        convertor.floatPart = x;
        convertor.intPart = 0x5f3759df - (convertor.intPart >> 1);
        return convertor.floatPart*(1.5f - 0.4999f*x*convertor.floatPart*convertor.floatPart);
}

/*
hgeVector *hgeVector::Normalize()
{
	float lenRcp;

	lenRcp=sqrtf(Dot(this));

	if(lenRcp)
	{
		lenRcp=1.0f/lenRcp;

		x*=lenRcp;
		y*=lenRcp;
	}

	return this;
}
*/

float hgeVector::Angle(const hgeVector *v) const
{
	if(v)
	{
		hgeVector s=*this, t=*v;

		s.Normalize(); t.Normalize();
		return acosf(s.Dot(t));
	}
	else return atan2f(y, x);
}

hgeVector *hgeVector::Rotate(float a)
{
	hgeVector v;

	v.x=x*cosf(a) - y*sinf(a);
	v.y=x*sinf(a) + y*cosf(a);

	x=v.x; y=v.y;

	return this;
}



