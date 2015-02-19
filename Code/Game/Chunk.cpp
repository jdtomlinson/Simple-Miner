#include <math.h>
#include <sstream>
#include "Chunk.hpp"
#include "../Engine/IntVector2.hpp"
#include "../Engine/MathFunctions.hpp"


//-----------------------------------------------------------------------------------------------
Chunk::Chunk()
	: m_position( 0.f, 0.f )
	, m_skyboxLightValue( 15 )
	, m_chunkHasBeenEdited( false )
	, m_updateLight( false )
	, m_updateBuffer( false )
{
	m_glRenderer.Initialize();
}


//-----------------------------------------------------------------------------------------------
Chunk::Chunk( const Vector2& position )
	: m_position( position )
	, m_skyboxLightValue( 15 )
	, m_chunkHasBeenEdited( false )
	, m_updateLight( false )
	, m_updateBuffer( false )
{
	m_glRenderer.Initialize();
}


//-----------------------------------------------------------------------------------------------
void Chunk::SetBlockChunk()
{
	int columnCount = NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS;
	for( int columnIndex = 0; columnIndex < columnCount; ++columnIndex )
	{
		float posX = (float) ( columnIndex&15 ) + m_position.x;
		float posY = (float) ( ( columnIndex>>4 )&15 ) + m_position.y;

		float groundLevel = PerlinNoise( posX, posY );
		groundLevel *= PERLIN_FLUCTUATION;
		groundLevel += ( (float) NUM_BLOCKS_PER_CHUNK_Z_AXIS * 0.455f );

		bool canSeeSky = true;

		for( int layerIndex = ( NUM_BLOCKS_PER_CHUNK_Z_AXIS - 1 ); layerIndex >= 0; --layerIndex )
		{
			float posZ = (float) ( ( layerIndex * columnCount )>>8 );

			unsigned char blockType;
			if( posZ > groundLevel )
				blockType = AIR;
			else if( posZ < STONE_MAX_BLOCKS_Z_AXIS )
			{
				blockType = STONE;
				canSeeSky = false;
			}
			else if( posZ < DIRT_MAX_BLOCKS_Z_AXIS )
			{
				blockType = DIRT;
				canSeeSky = false;
			}
			else
				blockType = AIR;

			int blockIndex = columnIndex + ( layerIndex * columnCount );

			m_blocks[ blockIndex ] = Block( blockType, Vector3( posX, posY, posZ ) );
			m_blocks[ blockIndex ].m_isUnderSky = canSeeSky;

			if( m_blocks[ blockIndex ].m_isUnderSky )
				m_blocks[ blockIndex ].m_internalLightValue = m_skyboxLightValue;
			else
				m_blocks[ blockIndex ].m_internalLightValue = 0;
		}
	}
}


//-----------------------------------------------------------------------------------------------
Block* Chunk::GetNorthBlock( Block* block )
{
	if( m_northChunk != nullptr && block->m_position.x == ( m_position.x + ( NUM_BLOCKS_PER_CHUNK_X_AXIS - 1 ) ) )
	{
		Vector3 northBlockPos = block->m_position;
		northBlockPos.x = m_northChunk->m_position.x;
		return &m_northChunk->m_blocks[ GetBlockIndex( m_northChunk->m_position, northBlockPos ) ];
	}
	else if( block->m_position.x < ( m_position.x + ( NUM_BLOCKS_PER_CHUNK_X_AXIS - 1 ) ) )
		return &m_blocks[ GetBlockIndex( m_position, block->m_position ) + 1 ];
	else
		return nullptr;
}


//-----------------------------------------------------------------------------------------------
Block* Chunk::GetSouthBlock( Block* block )
{
	if( m_southChunk != nullptr && block->m_position.x == m_position.x )
	{
		Vector3 southBlockPos = block->m_position;
		southBlockPos.x = ( NUM_BLOCKS_PER_CHUNK_X_AXIS - 1 ) + m_southChunk->m_position.x;
		return &m_southChunk->m_blocks[ GetBlockIndex( m_southChunk->m_position, southBlockPos ) ];
	}
	else if( block->m_position.x > m_position.x )
		return &m_blocks[ GetBlockIndex( m_position, block->m_position ) - 1 ];
	else
		return nullptr;
}


//-----------------------------------------------------------------------------------------------
Block* Chunk::GetWestBlock( Block* block )
{
	if( m_westChunk != nullptr && block->m_position.y == ( m_position.y + ( NUM_BLOCKS_PER_CHUNK_Y_AXIS - 1 ) ) )
	{
		Vector3 westBlockPos = block->m_position;
		westBlockPos.y = m_westChunk->m_position.y;
		return &m_westChunk->m_blocks[ GetBlockIndex( m_westChunk->m_position, westBlockPos ) ];
	}
	else if( block->m_position.y < ( m_position.y + ( NUM_BLOCKS_PER_CHUNK_Y_AXIS - 1 ) ) )
		return &m_blocks[ GetBlockIndex( m_position, block->m_position ) + NUM_BLOCKS_PER_CHUNK_X_AXIS ];
	else
		return nullptr;
}


//-----------------------------------------------------------------------------------------------
Block* Chunk::GetEastBlock( Block* block )
{
	if( m_eastChunk != nullptr && block->m_position.y == m_position.y )
	{
		Vector3 eastBlockPos = block->m_position;
		eastBlockPos.y = ( NUM_BLOCKS_PER_CHUNK_Y_AXIS - 1 ) + m_eastChunk->m_position.y;
		return &m_eastChunk->m_blocks[ GetBlockIndex( m_eastChunk->m_position, eastBlockPos ) ];
	}
	else if( block->m_position.y > m_position.y )
		return &m_blocks[ GetBlockIndex( m_position, block->m_position ) - NUM_BLOCKS_PER_CHUNK_X_AXIS ];
	else
		return nullptr;
}


//-----------------------------------------------------------------------------------------------
Block* Chunk::GetUpBlock( Block* block )
{
	if( block->m_position.z < ( NUM_BLOCKS_PER_CHUNK_Z_AXIS - 1 ) )
		return &m_blocks[ GetBlockIndex( m_position, block->m_position ) + ( NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS ) ];
	else
		return nullptr;
}


//-----------------------------------------------------------------------------------------------
Block* Chunk::GetDownBlock( Block* block )
{
	if( block->m_position.z > 0 )
		return &m_blocks[ GetBlockIndex( m_position, block->m_position ) - ( NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS ) ];
	else
		return nullptr;
}


//-----------------------------------------------------------------------------------------------
Vector2 Chunk::GetTexCoordForBlockType( unsigned char blockType )
{
	if( blockType == DIRT )
		return DIRT_TEX_COORDS;
	else if( blockType == STONE )
		return STONE_TEX_COORDS;
	else if( blockType == GLOWSTONE )
		return GLOWSTONE_TEX_COORDS;
	else
		return Vector2( 0.f, 0.f );
}


//-----------------------------------------------------------------------------------------------
void Chunk::SetSideCulling()
{
	int blockCount = NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS * NUM_BLOCKS_PER_CHUNK_Z_AXIS;

	for( int blockIndex = 0; blockIndex < blockCount; ++blockIndex )
	{
		Block* block = &m_blocks[ blockIndex ];
		CullBlock( block );
	}

	UpdateArrays();
}


//-----------------------------------------------------------------------------------------------
void Chunk::CullBlock( Block* block )
{
	if ( block->m_blockType == AIR )
		return;

	Vector3 pos = block->m_position;
	Vector2 texCoord = GetTexCoordForBlockType( block->m_blockType );
	float texCoordMinX = texCoord.x * ONE_OVER_THIRTY_TWO;
	float texCoordMaxX = texCoordMinX + ONE_OVER_THIRTY_TWO;
	float texCoordMinY = texCoord.y * ONE_OVER_THIRTY_TWO;
	float texCoordMaxY = texCoordMinY + ONE_OVER_THIRTY_TWO;

	Color color( 1.f, 1.f, 1.f );
	Block* southBlock = GetSouthBlock( block );
	if( southBlock != nullptr && southBlock->m_blockType == AIR )
	{
		color *= ( (float) southBlock->m_internalLightValue ) * ONE_OVER_FIFTEEN;
		color *= BLOCK_SIDES_COLOR_SCALE;
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y + 1.f, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y + 1.f, pos.z ), color, Vector2( texCoordMaxX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y, pos.z ), color, Vector2( texCoordMaxX, texCoordMinY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMinY ) ) );
	}

	color = Color( 1.f, 1.f, 1.f );
	Block* northBlock = GetNorthBlock( block );
	if( northBlock != nullptr && northBlock->m_blockType == AIR )
	{
		color *= ( (float) northBlock->m_internalLightValue ) * ONE_OVER_FIFTEEN;
		color *= BLOCK_SIDES_COLOR_SCALE;
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y, pos.z ), color, Vector2( texCoordMinX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y + 1.f, pos.z ), color, Vector2( texCoordMaxX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y + 1.f, pos.z + 1.f ), color, Vector2( texCoordMaxX, texCoordMinY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMinY ) ) );
	}

	color = Color( 1.f, 1.f, 1.f );
	Block* eastBlock = GetEastBlock( block );
	if( eastBlock != nullptr && eastBlock->m_blockType == AIR )
	{
		color *= ( (float) eastBlock->m_internalLightValue ) * ONE_OVER_FIFTEEN;
		color *= BLOCK_SIDES_COLOR_SCALE;
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y, pos.z ), color, Vector2( texCoordMaxX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y, pos.z ), color, Vector2( texCoordMaxX, texCoordMinY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMinY ) ) );
	}

	color = Color( 1.f, 1.f, 1.f );
	Block* westBlock = GetWestBlock( block );
	if( westBlock != nullptr && westBlock->m_blockType == AIR )
	{
		color *= ( (float) westBlock->m_internalLightValue ) * ONE_OVER_FIFTEEN;
		color *= BLOCK_SIDES_COLOR_SCALE;
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y + 1.f, pos.z ), color, Vector2( texCoordMinX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y + 1.f, pos.z + 1.f ), color, Vector2( texCoordMaxX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y + 1.f, pos.z + 1.f ), color, Vector2( texCoordMaxX, texCoordMinY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y + 1.f, pos.z ), color, Vector2( texCoordMinX, texCoordMinY ) ) );
	}

	color = Color( 1.f, 1.f, 1.f );
	Block* downBlock = GetDownBlock( block );
	if( downBlock != nullptr && downBlock->m_blockType == AIR )
	{
		color *= ( (float) downBlock->m_internalLightValue ) * ONE_OVER_FIFTEEN;
		color *= BLOCK_TOP_BOTTOM_COLOR_SCALE;
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y, pos.z ), color, Vector2( texCoordMinX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y + 1.f, pos.z ), color, Vector2( texCoordMaxX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y + 1.f, pos.z ), color, Vector2( texCoordMaxX, texCoordMinY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y, pos.z ), color, Vector2( texCoordMinX, texCoordMinY ) ) );
	}

	color = Color( 1.f, 1.f, 1.f );
	Block* upBlock = GetUpBlock( block );
	if( upBlock != nullptr && upBlock->m_blockType == AIR )
	{
		color *= ( (float) upBlock->m_internalLightValue ) * ONE_OVER_FIFTEEN;
		color *= BLOCK_TOP_BOTTOM_COLOR_SCALE;
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y + 1.f, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y + 1.f, pos.z + 1.f ), color, Vector2( texCoordMaxX, texCoordMaxY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x, pos.y, pos.z + 1.f ), color, Vector2( texCoordMaxX, texCoordMinY ) ) );
		m_blockVertices.push_back( Vertex( Vector3( pos.x + 1.f, pos.y, pos.z + 1.f ), color, Vector2( texCoordMinX, texCoordMinY ) ) );
	}
}


//-----------------------------------------------------------------------------------------------
int Chunk::GetBlockIndex( const Vector2& chunkPosition, const Vector3& blockPosition )
{
	int valueX = (int) ( floor( blockPosition.x ) - chunkPosition.x ) ;
	int valueY = (int) ( ( floor( blockPosition.y ) - chunkPosition.y ) * NUM_BLOCKS_PER_CHUNK_X_AXIS );
	int valueZ = (int) ( floor( blockPosition.z ) * ( NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS ) );

	return valueX + valueY + valueZ;
}


//-----------------------------------------------------------------------------------------------
Vector3 Chunk::GetBlockPositionFromIndex( int blockIndex )
{
	float posX = (float) ( blockIndex&15 ) + m_position.x;
	float posY = (float) ( ( blockIndex>>4 )&15 ) + m_position.y;
	float posZ = (float) ( ( blockIndex )>>8 );

	return Vector3( posX, posY, posZ );
}


//-----------------------------------------------------------------------------------------------
float Chunk::PerlinNoise( float posX, float posY )
{
	float southCoord = floor( posX * ONE_OVER_PERLIN_SAMPLE_SIZE_X );
	float northCoord = southCoord + 1.f;
	float eastCoord = floor( posY * ONE_OVER_PERLIN_SAMPLE_SIZE_Y );
	float westCoord = eastCoord + 1.f;
	float percentAcrossX = ( posX * ONE_OVER_PERLIN_SAMPLE_SIZE_X ) - southCoord;
	float percentAcrossY = ( posY * ONE_OVER_PERLIN_SAMPLE_SIZE_Y ) - eastCoord;
	percentAcrossX = SmoothStep( percentAcrossX );
	percentAcrossY = SmoothStep( percentAcrossY );

	Vector2 posSE( southCoord, eastCoord );
	Vector2 posSW( southCoord, westCoord );
	Vector2 posNE( northCoord, eastCoord );
	Vector2 posNW( northCoord, westCoord );

	float theta0 = GetRandomNoiseValue( posSE.x, posSE.y );
	float theta1 = GetRandomNoiseValue( posSW.x, posSW.y );
	float theta2 = GetRandomNoiseValue( posNE.x, posNE.y );
	float theta3 = GetRandomNoiseValue( posNW.x, posNW.y );

	Vector2 vecSE( ( 1.f - percentAcrossX ), ( 1.f - percentAcrossY ) );
	Vector2 vecSW( ( 1.f - percentAcrossX ), percentAcrossY );
	Vector2 vecNE( percentAcrossX, ( 1.f - percentAcrossY ) );
	Vector2 vecNW( percentAcrossX, percentAcrossY );

	vecSE.Normalize();
	vecSW.Normalize();
	vecNE.Normalize();
	vecNW.Normalize();

	float se = DotProduct( Vector2( cos( theta0 ), sin( theta0 ) ), vecSE );
	float sw = DotProduct( Vector2( cos( theta1 ), sin( theta1 ) ), vecSW );
	float ne = DotProduct( Vector2( cos( theta2 ), sin( theta2 ) ), vecNE );
	float nw = DotProduct( Vector2( cos( theta3 ), sin( theta3 ) ), vecNW );

	float south = ( se * ( 1.f - percentAcrossY ) ) + ( sw * percentAcrossY );
	float north = ( ne * ( 1.f - percentAcrossY ) ) + ( nw * percentAcrossY );

	return ( south * ( 1.f - percentAcrossX ) ) + ( north * percentAcrossX );
}


//-----------------------------------------------------------------------------------------------
void Chunk::SetBlocksInternalLightValue()
{
	for( int indexX = 0; indexX < NUM_BLOCKS_PER_CHUNK_X_AXIS; ++indexX )
	{
		for( int indexY = 0; indexY < NUM_BLOCKS_PER_CHUNK_Y_AXIS; ++indexY )
		{
			bool canSeeSky = true;
			for( int posZ = ( NUM_BLOCKS_PER_CHUNK_Z_AXIS - 1 ); posZ >= 0; --posZ )
			{
				float posX = (float) indexX + m_position.x;
				float posY = (float) indexY + m_position.y;
				int blockIndex = GetBlockIndex( m_position, Vector3( posX, posY, (float) posZ ) );
				if( m_blocks[ blockIndex ].m_blockType != AIR )
					canSeeSky = false;

				Block* block = &m_blocks[ blockIndex ];
				block->m_isUnderSky = canSeeSky;

				if( block->m_blockType == GLOWSTONE )
					block->m_internalLightValue = 12;
				else if( block->m_isUnderSky )
					block->m_internalLightValue = m_skyboxLightValue;
				else
					block->m_internalLightValue = 0;
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------
std::string Chunk::GetFileName()
{
	std::ostringstream convertPosX;
	std::ostringstream convertPosY;

	convertPosX << ( m_position.x * ONE_OVER_SIXTEEN );
	std::string posXString = convertPosX.str();
	convertPosY << ( m_position.y * ONE_OVER_SIXTEEN );
	std::string posYString = convertPosY.str();

	std::string fileName = "Data/Saved Chunks/posX" + posXString + "posY" + posYString + ".cnk";
	return fileName;
}


//-----------------------------------------------------------------------------------------------
bool Chunk::CheckIfFileExists()
{
	FILE* tempFile;
	errno_t checkForOpen;
	std::string fileString = GetFileName();
	char* fileName = (char*) fileString.c_str();
	checkForOpen = fopen_s( &tempFile, fileName, "rb" );
	if( checkForOpen != 0 )
		return false;

	fclose( tempFile );
	return true;
}


//-----------------------------------------------------------------------------------------------
void Chunk::SaveChunkToFile()
{
	FILE* chunkFile;
	std::string fileString = GetFileName();
	char* fileName = (char*) fileString.c_str();
	fopen_s( &chunkFile, fileName, "wb" );

	int blockCount = NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS * NUM_BLOCKS_PER_CHUNK_Z_AXIS;
	unsigned char blockType = m_blocks[ 0 ].m_blockType;
	unsigned short numOfSameBlockType = 0;
	for( int blockIndex = 0; blockIndex < blockCount; ++blockIndex )
	{
		Block block = m_blocks[ blockIndex ];
		if( block.m_blockType == blockType && blockIndex < ( blockCount - 1 ) )
		{
			++numOfSameBlockType;
			continue;
		}
		else if( blockIndex == ( blockCount - 1 ) )
			++numOfSameBlockType;

		fwrite( &blockType, sizeof( char ), 1, chunkFile );
		fwrite( &numOfSameBlockType, sizeof( short ), 1, chunkFile );
		blockType = block.m_blockType;
		numOfSameBlockType = 1;
	}

	fclose( chunkFile );
}


//-----------------------------------------------------------------------------------------------
void Chunk::LoadChunkFromFile()
{
	FILE* chunkFile;
	errno_t checkForOpen;
	std::string fileString = GetFileName();
	char* fileName = (char*) fileString.c_str();
	checkForOpen = fopen_s( &chunkFile, fileName, "rb" );
	if( checkForOpen != 0 )
		return;

	int blockIndex = 0;
	int endOfLoop = 0;
	int maxNumBlocks = NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS * NUM_BLOCKS_PER_CHUNK_Z_AXIS;
	while( !feof( chunkFile ) && blockIndex < maxNumBlocks )
	{
		unsigned char blockType;
		fread( &blockType, sizeof( char ), 1, chunkFile );
		short numOfSameBlockType;
		fread( &numOfSameBlockType, sizeof( short ), 1, chunkFile );
		endOfLoop += numOfSameBlockType;

		for( blockIndex; blockIndex < endOfLoop; ++blockIndex )
		{
			Vector3 blockPosition = GetBlockPositionFromIndex( blockIndex );
			m_blocks[ blockIndex ] = Block( blockType, blockPosition );
		}
	}

	fclose( chunkFile );

	SetBlocksInternalLightValue();
}


//-----------------------------------------------------------------------------------------------
void Chunk::UpdateArrays()
{
	if( m_blockVertices.size() == 0 )
		return;

	m_glRenderer.DeleteBuffers( 1, &m_VBOid );

	m_glRenderer.GenerateBuffers( 1, &m_VBOid );
	m_glRenderer.BindBuffer( m_VBOid );
	m_glRenderer.SetBufferData( m_blockVertices.size() * sizeof( Vertex ), &m_blockVertices.front() );
}


//-----------------------------------------------------------------------------------------------
void Chunk::Render()
{
	m_glRenderer.PushMatrix();

	m_glRenderer.BindBuffer( m_VBOid );

	m_glRenderer.SetVertexPointer( 3, sizeof( Vertex ), (float*) offsetof( Vertex, m_position ) );
	m_glRenderer.SetColorPointer( 4, sizeof( Vertex ), (float*) offsetof( Vertex, m_color ) );
	m_glRenderer.SetTexCoordPointer( 2, sizeof( Vertex ), (float*) offsetof( Vertex, m_texCoords ) );

	m_glRenderer.EnableClientState( VERTEX_ARRAY );
	m_glRenderer.EnableClientState( COLOR_ARRAY );
	m_glRenderer.EnableClientState( TEXTURE_COORD_ARRAY );

	m_glRenderer.EnableCullFace();
	m_glRenderer.CullFrontFaceDirection( COUNTER_CLOCKWISE );
	m_glRenderer.CullFaceSide( BACK );

	m_glRenderer.EnableTexture2D();
	m_glRenderer.BindTexture2D( m_texAtlas->m_openglTextureID );

	m_glRenderer.DrawArrays( QUADS, 0, m_blockVertices.size() );

	m_glRenderer.DisableClientState( VERTEX_ARRAY );
	m_glRenderer.DisableClientState( COLOR_ARRAY );
	m_glRenderer.DisableClientState( TEXTURE_COORD_ARRAY );

	m_glRenderer.DisableTexture2D();

	m_glRenderer.PopMatrix();
}