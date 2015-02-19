#ifndef include_Vector3
#define include_Vector3
#pragma once

//-----------------------------------------------------------------------------------------------
#include <math.h>


//-----------------------------------------------------------------------------------------------
struct Vector3
{
public:
	Vector3() {}
	Vector3( float initialX, float initialY, float initialZ ) : x( initialX ), y ( initialY ), z( initialZ ) {}
	Vector3 operator+( const Vector3& vec ) const;
	void operator+=( const Vector3& vec );
	Vector3 operator-( const Vector3& vec ) const;
	void operator-=( const Vector3& vec );
	Vector3 operator*( float val ) const;
	void operator*=( float val );
	bool operator==( const Vector3& vec ) const;
	bool operator!=( const Vector3& vec ) const;
	bool operator<( const Vector3& vec ) const;
	bool operator>( const Vector3& vec ) const;
	float GetLength();
	void Normalize();

	float x;
	float y;
	float z;
};


//-----------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator+( const Vector3& vec ) const
{
	return Vector3( this->x + vec.x, this->y + vec.y, this->z + vec.z );
}


//-----------------------------------------------------------------------------------------------
inline void Vector3::operator+=( const Vector3& vec )
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
}


//-----------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator-( const Vector3& vec ) const
{
	return Vector3( this->x - vec.x, this->y - vec.y, this->z - vec.z );
}


//-----------------------------------------------------------------------------------------------
inline void Vector3::operator-=( const Vector3& vec )
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
}


//-----------------------------------------------------------------------------------------------
inline Vector3 Vector3::operator*( float val ) const
{
	return Vector3( this->x * val, this->y * val, this->z * val );
}


//-----------------------------------------------------------------------------------------------
inline void Vector3::operator*=( float val )
{
	this->x *= val;
	this->y *= val;
	this->z *= val;
}


//-----------------------------------------------------------------------------------------------
inline bool Vector3::operator==( const Vector3& vec ) const
{
	if( this->x == vec.x && this->y == vec.y && this->z == vec.z )
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
inline bool Vector3::operator!=( const Vector3& vec ) const
{
	if( this->x != vec.x || this->y != vec.y || this->z != vec.z )
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
inline bool Vector3::operator<( const Vector3& vec ) const
{
	if( this->x < vec.x )
		return true;
	else if( this->x == vec.x )
	{
		if( this->y < vec.y )
			return true;
		else if( this->y == vec.y )
		{
			if( this->z < vec.z )
				return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------------------
inline bool Vector3::operator>( const Vector3& vec ) const
{
	if( this->x > vec.x )
		return true;
	else if( this->x == vec.x )
	{
		if( this->y > vec.y )
			return true;
		else if( this->y == vec.y )
		{
			if( this->z > vec.z )
				return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------------------
inline float Vector3::GetLength()
{
	return sqrt( ( this->x * this->x ) + ( this->y * this->y ) + ( this->z * this->z ) );
}


//-----------------------------------------------------------------------------------------------
inline void Vector3::Normalize()
{
	float length = this->GetLength();
	float oneOverLen = 1.f;

	if( length != 0.f )
		oneOverLen = 1.f / length ;

	this->x *= oneOverLen;
	this->y *= oneOverLen;
	this->z *= oneOverLen;
}


#endif // include_Vector3