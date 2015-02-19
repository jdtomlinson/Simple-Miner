#ifndef include_Chunk
#define include_Chunk
#pragma once

//-----------------------------------------------------------------------------------------------
#include <map>
#include <vector>
#include "Block.hpp"
#include "../Engine/Vector2.hpp"
#include "../Engine/Vertex.hpp"
#include "../Engine/OpenGLRenderer.hpp"


//-----------------------------------------------------------------------------------------------
const int NUM_BLOCKS_PER_CHUNK_X_AXIS = 16;
const int NUM_BLOCKS_PER_CHUNK_Y_AXIS = 16;
const int NUM_BLOCKS_PER_CHUNK_Z_AXIS = 128;
const int NUM_BLOCKS_PER_CHUNK = NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS * NUM_BLOCKS_PER_CHUNK_Z_AXIS;
const int PERLIN_SAMPLE_SIZE_X = 40;
const int PERLIN_SAMPLE_SIZE_Y = 40;
const int NUM_VERTICES_PER_CHUNK = NUM_BLOCKS_PER_CHUNK * 24;
const int STONE_MAX_BLOCKS_Z_AXIS = NUM_BLOCKS_PER_CHUNK_Z_AXIS / 2;
const int DIRT_MAX_BLOCKS_Z_AXIS = ( NUM_BLOCKS_PER_CHUNK_Z_AXIS / 3 ) * 2;
const float BLOCK_SIDES_COLOR_SCALE = 0.8f;
const float BLOCK_TOP_BOTTOM_COLOR_SCALE = 1.f;
const float PERLIN_FLUCTUATION = 10.f;
const float ONE_OVER_PERLIN_SAMPLE_SIZE_X = 1.f / (float) PERLIN_SAMPLE_SIZE_X;
const float ONE_OVER_PERLIN_SAMPLE_SIZE_Y = 1.f / (float) PERLIN_SAMPLE_SIZE_Y;
const Vector2 DIRT_TEX_COORDS = Vector2( 6.f, 12.f );
const Vector2 STONE_TEX_COORDS = Vector2( 5.f, 11.f );
const Vector2 GLOWSTONE_TEX_COORDS = Vector2( 19.f, 22.f );


//-----------------------------------------------------------------------------------------------
class Chunk
{
public:
	Chunk();
	Chunk( const Vector2& position );
	void SetBlockChunk();
	Block* GetNorthBlock( Block* block );
	Block* GetSouthBlock( Block* block );
	Block* GetWestBlock( Block* block );
	Block* GetEastBlock( Block* block );
	Block* GetUpBlock( Block* block );
	Block* GetDownBlock( Block* block );
	Vector2 GetTexCoordForBlockType( unsigned char blockType );
	void SetSideCulling();
	void CullBlock( Block* block );
	int GetBlockIndex( const Vector2& chunkPosition, const Vector3& blockPosition );
	Vector3 GetBlockPositionFromIndex( int blockIndex );
	float PerlinNoise( float posX, float posY );
	void SetBlocksInternalLightValue();
	std::string GetFileName();
	bool CheckIfFileExists();
	void SaveChunkToFile();
	void LoadChunkFromFile();
	void UpdateArrays();
	void Render();

	Vector2					m_position;
	Block					m_blocks[ NUM_BLOCKS_PER_CHUNK ];
	std::vector<Vertex>		m_blockVertices;
	unsigned int			m_VBOid;
	unsigned char			m_skyboxLightValue;
	bool					m_chunkHasBeenEdited;
	bool					m_updateLight;
	bool					m_updateBuffer;
	Chunk*					m_northChunk;
	Chunk*					m_southChunk;
	Chunk*					m_westChunk;
	Chunk*					m_eastChunk;
	Texture*				m_texAtlas;
	OpenGLRenderer			m_glRenderer;
};


#endif // include_Chunk