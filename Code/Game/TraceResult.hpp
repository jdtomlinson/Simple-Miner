#ifndef include_TraceResult
#define include_TraceResult
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Block.hpp"
#include "Chunk.hpp"


//-----------------------------------------------------------------------------------------------
enum Directions {
	NONE = -1,
	NORTH,
	SOUTH,
	WEST,
	EAST,
	UP,
	DOWN,
};


//-----------------------------------------------------------------------------------------------
struct TraceResult
{
public:
	TraceResult() {}
	TraceResult( Block* block, Directions direction ) : m_blockFirstHit( block ), m_blockFaceImpact( direction ) {}

	Block*			m_blockFirstHit;
	Directions		m_blockFaceImpact;
};



#endif // include_TraceResult