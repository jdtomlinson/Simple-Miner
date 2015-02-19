#ifndef include_Color
#define include_Color
#pragma once

struct Color
{
	Color() : r( 0.f ), g( 0.f ), b( 0.f ), a( 1.f ) {}
	Color( float red, float green, float blue ) : r( red ), g( green ), b( blue ), a( 1.f ) {}
	Color( float red, float green, float blue, float alpha ) : r( red ), g( green ), b( blue ), a( alpha ) {}
	bool operator==( const Color& color ) const;
	void operator+=( const Color& color );
	void operator-=( const Color& color );
	void operator*=( float val );
	void Clamp();

	float r;
	float g;
	float b;
	float a;
};


//-----------------------------------------------------------------------------------------------
inline bool Color::operator==( const Color& color ) const
{
	if( this->r == color.r && this->g == color.g && this->b == color.b && this->a && color.a )
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------------------------
inline void Color::operator+=( const Color& color )
{
	this->r += color.r;
	this->g += color.g;
	this->b += color.b;
	this->Clamp();
}


//-----------------------------------------------------------------------------------------------
inline void Color::operator-=( const Color& color )
{
	this->r -= color.r;
	this->g -= color.g;
	this->b -= color.b;
	this->Clamp();
}


//-----------------------------------------------------------------------------------------------
inline void Color::operator*=( float val )
{
	this->r *= val;
	this->g *= val;
	this->b *= val;
	this->Clamp();
}


//-----------------------------------------------------------------------------------------------
inline void Color::Clamp()
{
	/*if( this->r < 0.f )
		this->r = 0.f;
	if( this->g < 0.f )
		this->g = 0.f;
	if( this->b < 0.f )
		this->b = 0.f;
	if( this->a < 0.f )
		this->a = 0.f;*/

	if( this->r > 1.f )
		this->r = 1.f;
	if( this->g > 1.f )
		this->g = 1.f;
	if( this->b > 1.f )
		this->b = 1.f;
	if( this->a > 1.f )
		this->a = 1.f;
}


#endif // include_Color