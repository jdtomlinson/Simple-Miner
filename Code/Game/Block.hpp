#ifndef include_Block
#define include_Block
#pragma once

//-----------------------------------------------------------------------------------------------
#include "../Engine/Color.hpp"
#include "../Engine/Vector3.hpp"
#include "../Engine/Texture.hpp"
#include "../Engine/OpenGLRenderer.hpp"


//-----------------------------------------------------------------------------------------------
const float ONE_OVER_THIRTY_TWO = 1.f / 32.f;
const unsigned char AIR = 0;
const unsigned char DIRT = 1;
const unsigned char STONE = 2;
const unsigned char GLOWSTONE = 3;


//-----------------------------------------------------------------------------------------------
enum cullFaces {
	CULL_NEGATIVE_X_FACE = 0x01,
	CULL_POSITIVE_X_FACE = 0x02,
	CULL_NEGATIVE_Y_FACE = 0x04,
	CULL_POSITIVE_Y_FACE = 0x08,
	CULL_NEGATIVE_Z_FACE = 0x10,
	CULL_POSITIVE_Z_FACE = 0x20,
};


//-----------------------------------------------------------------------------------------------
class Block
{
public:
	Block();
	Block( unsigned char blockType, Vector3 centerPosition );
	void RenderOutline();
	
	unsigned char		m_blockType;
	unsigned char		m_highlightFaceFlag;
	unsigned char		m_internalLightValue;
	bool				m_isUnderSky;
	Vector3				m_position;
	OpenGLRenderer		m_glRenderer;
};


#endif // include_Block