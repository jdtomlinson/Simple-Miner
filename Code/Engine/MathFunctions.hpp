#ifndef included_MathFunction
#define included_MathFunction
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Vector2.hpp"
#include "../Game/GameCommon.hpp"


//-----------------------------------------------------------------------------------------------
inline bool FloatsApproximatelyEqual( float value1, float value2 )
{
	if( value1 < value2 + MARGIN_OF_ERROR && value1 > value2 - MARGIN_OF_ERROR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
inline float ConvertRadiansToDegrees( float rad )
{
	return ( rad * ( 180.f * ONE_OVER_PI) );
}


//-----------------------------------------------------------------------------------------------
inline float ConvertDegreesToRadians( float deg )
{
	return ( deg * ( pi * ONE_OVER_ONE_HUNDRED_EIGHTY ) );
}


//-----------------------------------------------------------------------------------------------
inline float GetSquaredDistanceBetweenPoints2D( const Vector2& point1, const Vector2& point2 )
{
	float differenceX = point1.x - point2.x;
	float differenceY = point1.y - point2.y;
	return ( ( differenceX * differenceX ) + ( differenceY * differenceY ) );
}


//-----------------------------------------------------------------------------------------------
inline float GetRandomNoiseValue( float x, float y )
{
	int n = int(x + ( int(y) * 57 ));
	n = ( n<<13 )^n;
	return ( 1.f - (float) ( ( n * ( n * n * 15731 + 789221 ) + 1376312589 ) & 0x7fffffff ) / 1073741824.f );
}


//-----------------------------------------------------------------------------------------------
inline float SmoothStep( float p )
{
	return ( 3 * ( p * p ) ) - ( 2 * ( p * p * p ) );
}


//-----------------------------------------------------------------------------------------------
inline float DotProduct( const Vector2& vec1, const Vector2& vec2 )
{
	return ( vec1.x * vec2.x ) + ( vec1.y * vec2.y );
}


#endif // include_MathFunctions