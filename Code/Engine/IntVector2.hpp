#ifndef include_IntVector2
#define include_IntVector2
#pragma once

//-----------------------------------------------------------------------------------------------
struct IntVector2
{
public:
	IntVector2() {}
	IntVector2( int initialX, int initialY ) : x( initialX ), y ( initialY ) {}
	bool operator==( const IntVector2& vec ) const;
	bool operator<( const IntVector2& vec ) const;
	bool operator>( const IntVector2& vec ) const;

	int x;
	int y;
};


//-----------------------------------------------------------------------------------------------
inline bool IntVector2::operator==( const IntVector2& vec ) const
{
	if( this->x == vec.x && this->y == vec.y )
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
inline bool IntVector2::operator<( const IntVector2& vec ) const
{
	if( this->y < vec.y )
		return true;
	else if( this->y == vec.y )
	{
		if( this->x < vec.x )
			return true;
	}
	
	return false;
}


//-----------------------------------------------------------------------------------------------
inline bool IntVector2::operator>( const IntVector2& vec ) const
{
	if( this->y > vec.y )
		return true;
	else if( this->y == vec.y )
	{
		if( this->x > vec.x )
			return true;
	}

	return false;
}


#endif // include_IntVector2