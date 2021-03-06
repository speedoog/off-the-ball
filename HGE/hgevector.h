/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hgeVector helper class
*/


#ifndef HGEVECTOR_H
#define HGEVECTOR_H


#include "hge.h"
#include <math.h>


/*
** Fast 1.0/sqrtf(float) routine
*/
float InvSqrt(float x);

class hgeVector
{
public:
	static hgeVector ZERO;
	static hgeVector UNIT_X;
	static hgeVector UNIT_Y;

	float	x,y;

	inline				hgeVector(float _x, float _y)			{ x=_x; y=_y; }
	inline				hgeVector()								{ x=0; y=0; }

	inline	hgeVector	operator-  ()					const 	{ return hgeVector(-x, -y);		}
	inline	hgeVector	operator-  (const hgeVector &v) const 	{ return hgeVector(x-v.x, y-v.y); }
	inline	hgeVector	operator+  (const hgeVector &v) const 	{ return hgeVector(x+v.x, y+v.y); }
	inline	hgeVector&	operator-= (const hgeVector &v)		  	{ x-=v.x; y-=v.y; return *this;	}
	inline	hgeVector&	operator+= (const hgeVector &v)		  	{ x+=v.x; y+=v.y; return *this;	}
	inline	bool		operator== (const hgeVector &v)	const 	{ return (x==v.x && y==v.y);		}
	inline	bool		operator!= (const hgeVector &v)	const 	{ return (x!=v.x || y!=v.y);		}

	inline	hgeVector	operator/  (const float scalar)	const 	{ return hgeVector(x/scalar, y/scalar); }
	inline	hgeVector	operator*  (const float scalar) const 	{ return hgeVector(x*scalar, y*scalar); }
	inline	hgeVector&	operator*= (const float scalar)		  	{ x*=scalar; y*=scalar; return *this;   }

	inline	float		Dot(const hgeVector& v) const			{ return x*v.x + y*v.y; }
	inline	float		Cross(const hgeVector& v) const 		{ return x*v.y - y*v.x;	}
	inline	float		LengthSq() const						{ return Dot(*this);	}
	inline	float		Length() const							{ return sqrtf(LengthSq()); }
			float		Angle(const hgeVector *v = 0) const;
	
	inline	void		Clamp(const float max)					{ if(Length() > max)	{ Normalize(); x *= max; y *= max; } }
	inline	hgeVector*	Normalize()								{ float rc=InvSqrt(Dot(*this)); x*=rc; y*=rc; return this; }
			hgeVector*	Rotate(float a);
	inline	void		SetPolar(const float rAmplitude, const float rAngle)	{ x =rAmplitude*cosf(rAngle); y =rAmplitude*sinf(rAngle); }
};

inline hgeVector operator* (const float s, const hgeVector &v)		{ return v*s; }
inline float	 operator^ (const hgeVector &v, const hgeVector &u) { return v.Angle(&u); }
inline float	 operator% (const hgeVector &v, const hgeVector &u) { return v.Dot(u); }


#endif	//HGEVECTOR_H
