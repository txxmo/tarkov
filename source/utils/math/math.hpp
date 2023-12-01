#pragma once

#include <xmmintrin.h>
#include <emmintrin.h>
#include <cmath>

class vector2
{
public:
	float x, y;
public:
	vector2( ) { this->x = this->y = 0; }
	vector2( float x, float y )
	{
		this->x = x;
		this->y = y;
	}
};

class vector3
{
public:
	float x, y, z;
public:
	vector3( )
	{
		x = y = z = 0;
	}
	vector3( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	float length( )
	{
		return sqrt( x * x + y * y + z * z );
	}

	float distance_to( const vector3 v )
	{
		return vector3( this->x - v.x, this->y - v.y, this->z - v.z ).length( );
	}
};

struct matrix3x4_t
{
	std::byte vec0[16];
	std::byte vec1[16];
	std::byte vec2[16];
};

namespace math
{
	//vector3 get_transform_pos( const uintptr_t transform );
}