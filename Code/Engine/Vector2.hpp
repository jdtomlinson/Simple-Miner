#ifndef include_Vector2
#define include_Vector2
#pragma once

//-----------------------------------------------------------------------------------------------
#include <math.h>


//-----------------------------------------------------------------------------------------------
struct Vector2
{
public:
	Vector2() {}
	Vector2( float initialX, float initialY ) : x( initialX ), y ( initialY ) {}
	Vector2 operator+( const Vector2& vec ) const;
	void operator+=( const Vector2& vec );
	Vector2 operator-( const Vector2& vec ) const;
	bool operator==( const Vector2& vec ) const;
	bool operator!=( const Vector2& vec ) const;
	void operator*=( float val );
	void Normalize();

	float x;
	float y;
};


//-----------------------------------------------------------------------------------------------
inline Vector2 Vector2::operator+( const Vector2& vec ) const
{
	return Vector2( this->x + vec.x, this->y + vec.y );
}


//-----------------------------------------------------------------------------------------------
inline void Vector2::operator+=( const Vector2& vec )
{
	this->x += vec.x;
	this->y += vec.y;
}


//-----------------------------------------------------------------------------------------------
inline Vector2 Vector2::operator-( const Vector2& vec ) const
{
	return Vector2( this->x - vec.x, this->y - vec.y );
}


//-----------------------------------------------------------------------------------------------
inline bool Vector2::operator==( const Vector2& vec ) const
{
	if( this->x == vec.x && this->y == vec.y )
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
inline bool Vector2::operator!=( const Vector2& vec ) const
{
	if( this->x != vec.x || this->y != vec.y )
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
inline void Vector2::operator*=( float val )
{
	this->x *= val;
	this->y *= val;
}


//-----------------------------------------------------------------------------------------------
inline void Vector2::Normalize()
{
	float lenSquared = ( ( this->x * this->x ) + ( this->y * this->y ) );
	float oneOverLen = 1.f;

	if( lenSquared != 0.f )
		oneOverLen = 1.f / sqrt( lenSquared );

	this->x *= oneOverLen;
	this->y *= oneOverLen;
}


#endif // include_Vector2