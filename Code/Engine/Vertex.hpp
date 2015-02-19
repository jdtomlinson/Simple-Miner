#ifndef include_Vertex
#define include_Vertex
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Color.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"


struct Vertex
{
public:
	Vertex() {}
	Vertex( Vector3 pos, Color col, Vector2 tex ) : m_position( pos.x, pos.y, pos.z ), m_color( col.r, col.g, col.b, col.a ), m_texCoords( tex.x, tex.y ) {}
	bool operator==( const Vertex& vert ) const;

	Vector3		m_position;
	Color		m_color;
	Vector2		m_texCoords;
};


//-----------------------------------------------------------------------------------------------
inline bool Vertex::operator==( const Vertex& vert ) const
{
	if( this->m_position == vert.m_position && this->m_color == vert.m_color && this->m_texCoords == vert.m_texCoords )
		return true;
	else
		return false;
}


#endif // include_Vertex