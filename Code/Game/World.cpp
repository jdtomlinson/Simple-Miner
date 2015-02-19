#include "time.h"
#include "World.hpp"
#include "../Engine/MathFunctions.hpp"


//-----------------------------------------------------------------------------------------------
World::World( float worldWith, float worldHeight ) : m_size( worldWith, worldHeight)
{
	
}


//-----------------------------------------------------------------------------------------------
FMOD::Sound* World::LoadAudio( char* fileName )
{
	FMOD::Sound* sound;
	m_audioSystem->createSound( fileName, FMOD_HARDWARE, 0, &sound );
	sound->setMode( FMOD_LOOP_OFF );
	return sound;
}


//-----------------------------------------------------------------------------------------------
void World::InitializeAudio()
{
	FMOD::System_Create( &m_audioSystem );
	m_audioSystem->init( 32, FMOD_INIT_NORMAL, 0 );

	m_musicSounds.push_back( LoadAudio( "Data/Audio/Music0.mp3" ) );
	m_musicSounds.push_back( LoadAudio( "Data/Audio/Music1.mp3" ) );
	m_musicSounds.push_back( LoadAudio( "Data/Audio/Music2.mp3" ) );

	m_walkDirtSounds.push_back( LoadAudio( "Data/Audio/walkDirt0.mp3" ) );
	m_walkDirtSounds.push_back( LoadAudio( "Data/Audio/walkDirt1.mp3" ) );
	m_walkDirtSounds.push_back( LoadAudio( "Data/Audio/walkDirt2.mp3" ) );
	m_walkDirtSounds.push_back( LoadAudio( "Data/Audio/walkDirt3.mp3" ) );

	m_walkStoneSounds.push_back( LoadAudio( "Data/Audio/walkStone0.mp3" ) );
	m_walkStoneSounds.push_back( LoadAudio( "Data/Audio/walkStone1.mp3" ) );
	m_walkStoneSounds.push_back( LoadAudio( "Data/Audio/walkStone2.mp3" ) );
	m_walkStoneSounds.push_back( LoadAudio( "Data/Audio/walkStone3.mp3" ) );
	m_walkStoneSounds.push_back( LoadAudio( "Data/Audio/walkStone4.mp3" ) );

	m_digDirtSounds.push_back( LoadAudio( "Data/Audio/digDirt0.mp3" ) );
	m_digDirtSounds.push_back( LoadAudio( "Data/Audio/digDirt1.mp3" ) );
	m_digDirtSounds.push_back( LoadAudio( "Data/Audio/digDirt2.mp3" ) );
	m_digDirtSounds.push_back( LoadAudio( "Data/Audio/digDirt3.mp3" ) );

	m_digStoneSounds.push_back( LoadAudio( "Data/Audio/digStone0.mp3" ) );
	m_digStoneSounds.push_back( LoadAudio( "Data/Audio/digStone1.mp3" ) );
	m_digStoneSounds.push_back( LoadAudio( "Data/Audio/digStone2.mp3" ) );
	m_digStoneSounds.push_back( LoadAudio( "Data/Audio/digStone3.mp3" ) );

	m_placeSound = LoadAudio( "Data/Audio/placeBlock.mp3" );
	m_stayNoClipSound = LoadAudio( "Data/Audio/noChange.mp3" );
}


//-----------------------------------------------------------------------------------------------
void World::Initialize()
{
	m_movementMode = WALKING;
	m_playerPrevPosition = BOUNDING_BOX_START_POSITION;
	m_isOnGround = false;
	m_isMoving = false;
	m_initialized = false;
	m_changedToDay = true;
	m_changedToNight = false;
	m_fallSpeed = 0.f;
	m_timeOfDay = TIME_OF_DAY_AT_START;
	m_secondsSinceMoveStart = 0.f;
	m_secondsSinceMoveStop = 0.f;
	m_secondsSinceSongEnd = 30.f;
	m_numChunksInitialized = 0.f;
	m_prevCameraForwardXY = Vector3();
	m_prevcameraLeftXY = Vector3();

	m_playerBox = AABB3( BOUNDING_BOX_START_POSITION, BOUNDING_BOX_X_AXIS_LENGTH, BOUNDING_BOX_Y_AXIS_LENGTH, BOUNDING_BOX_Z_AXIS_LENGTH );
	Vector3 cameraStartPosition = BOUNDING_BOX_START_POSITION;
	cameraStartPosition.z += CAMERA_DISTANCE_FROM_BOX_BOTTOM - m_playerBox.m_zAxisHalfLength;
	m_camera = Camera( cameraStartPosition );
	m_mouse = Mouse();
	m_mouse.SetCursorPosition( Vector2( m_size.x * 0.5f, m_size.y * 0.5f ) );
	m_keyboard = Keyboard();

	m_skyboxTexture = Texture::GetTextureByName( "Data/Images/Skybox.png" );
	m_loadScreenTexture = Texture::GetTextureByName( "Data/Images/LoadScreen.png" );
	m_loadingBar = LoadingBar( LOADING_BAR_BOTTOM_LEFT_POSITION, LOADING_BAR_TOP_RIGHT_POSITION, LOADING_BAR_COLOR );

	InitializeAudio();
}


//-----------------------------------------------------------------------------------------------
void World::Deconstruct()
{
	typedef std::map<IntVector2, Chunk*>::iterator mapIterator;
	for( mapIterator iter = m_chunks.begin(); iter != m_chunks.end(); ++iter )
	{
		Chunk* blockChunk = iter->second;
		if( blockChunk->m_chunkHasBeenEdited )
			blockChunk->SaveChunkToFile();

		delete blockChunk;
	}

	m_chunks.clear();
}


//-----------------------------------------------------------------------------------------------
bool World::ProcessKeyDownEvent( unsigned char virtualKeyCode )
{
	m_keyboard.SetKeyDown( virtualKeyCode );
	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::ProcessKeyUpEvent( unsigned char virtualKeyCode )
{
	m_keyboard.SetKeyUp( virtualKeyCode );
	return false;
}


//-----------------------------------------------------------------------------------------------
Block* World::GetBlock( const Vector3& blockPosition )
{
	if( blockPosition.z < 0 || blockPosition.z > ( NUM_BLOCKS_PER_CHUNK_Z_AXIS - 1 ) )
		return nullptr;

	int chunkPosX = ( (int) floor( blockPosition.x ) )>>4;
	int chunkPosY = ( (int) floor( blockPosition.y ) )>>4;
	IntVector2 chunkPos( chunkPosX, chunkPosY );

	if( m_chunks.find( chunkPos ) == m_chunks.end() )
		return nullptr;

	Chunk* blockChunk = m_chunks[ chunkPos ];
	if( blockChunk == nullptr )
		return nullptr;

	int index = blockChunk->GetBlockIndex( blockChunk->m_position, blockPosition );
	return &blockChunk->m_blocks[ index ];
}


//-----------------------------------------------------------------------------------------------
Chunk* World::GetChunkBlockIsIn( Block* block )
{
	int chunkPosX = ( (int) block->m_position.x )>>4;
	int chunkPosY = ( (int) block->m_position.y )>>4;
	IntVector2 chunkPos( chunkPosX, chunkPosY );

	 if( m_chunks.find( chunkPos ) == m_chunks.end() )
		 return nullptr;

	 return m_chunks[ chunkPos ];
}


//-----------------------------------------------------------------------------------------------
TraceResult World::CastRay( const Vector3& start, const Vector3& end )
{
	Vector3 displacement( end.x - start.x, end.y - start.y, end.z - start.z );
	displacement.Normalize();
	std::set<float> distances;

	int numX = (int) abs( floor( end.x ) - floor( start.x ) );
	for( int countX = 0; countX < numX; ++countX )
	{
		float posX = 0.f;
		if( displacement.x < 0.f )
			posX = floor( start.x ) - (float) countX;
		else
			posX = ceil( start.x ) + (float) countX;

		distances.insert( abs( ( posX - start.x ) / displacement.x ) );
	}

	int numY = (int) abs( floor( end.y ) - floor( start.y ) );
	for( int countY = 0; countY < numY; ++countY )
	{
		float posY = 0.f;
		if( displacement.y < 0.f )
			posY = floor( start.y ) - (float) countY;
		else
			posY = ceil( start.y ) + (float) countY;

		distances.insert( abs( ( posY - start.y ) / displacement.y ) );
	}

	int numZ = (int) abs( floor( end.z ) - floor( start.z ) );
	for( int countZ = 0; countZ < numZ; ++countZ )
	{
		float posZ = 0.f;
		if( displacement.z < 0.f )
			posZ = floor( start.z ) - (float) countZ;
		else
			posZ = ceil( start.z ) + (float) countZ;

		distances.insert( abs( ( posZ - start.z ) / displacement.z ) );
	}

	Block* blockStartIsIn = GetBlock( start );

	std::set<float>::iterator iter;
	for( iter = distances.begin(); iter != distances.end(); ++iter )
	{
		Directions faceDirection = NONE;
		Vector3 point = start + ( displacement * *iter );

		if( displacement.x < 0.f && point.x == floor( point.x ) )
		{
			point.x -= 1.f;
			faceDirection = NORTH;
		}
		else if( displacement.x > 0.f && point.x == floor( point.x ) )
		{
			faceDirection = SOUTH;
		}
		else if( displacement.y < 0.f && point.y == floor( point.y ) )
		{
			point.y -= 1.f;
			faceDirection = WEST;
		}
		else if( displacement.y > 0.f && point.y == floor( point.y ) )
		{
			faceDirection = EAST;
		}
		else if( displacement.z < 0.f && point.z == floor( point.z ) )
		{
			point.z -= 1.f;
			faceDirection = UP;
		}
		else if( displacement.z > 0.f && point.z == floor( point.z ) )
		{
			faceDirection = DOWN;
		}
		
		Block* block = GetBlock( point );
		if( block != nullptr && block->m_blockType != AIR && block != blockStartIsIn )
			return TraceResult( block, faceDirection );
	}

	return TraceResult( nullptr, NONE );
}


//-----------------------------------------------------------------------------------------------
void World::SetCameraPositionAndOrientation()
{
	m_glRenderer.Rotatef( -90.f, 1.f, 0.f, 0.f );
	m_glRenderer.Rotatef( 90.f, 0.f, 0.f, 1.f );

	m_glRenderer.Rotatef( -m_camera.m_orientation.roll, 1.f, 0.f, 0.f );
	m_glRenderer.Rotatef( -m_camera.m_orientation.pitch, 0.f, 1.f, 0.f );
	m_glRenderer.Rotatef( -m_camera.m_orientation.yaw, 0.f, 0.f, 1.f );

	m_glRenderer.Translatef( -m_camera.m_position.x, -m_camera.m_position.y, -m_camera.m_position.z );
}


//-----------------------------------------------------------------------------------------------
void World::SetSurroundingChunks( Chunk* blockChunk )
{
	if( blockChunk == nullptr )
		return;

	IntVector2 chunkPosition( (int) ( blockChunk->m_position.x * ONE_OVER_SIXTEEN ), (int) ( blockChunk->m_position.y * ONE_OVER_SIXTEEN ) );
	IntVector2 northPosition( chunkPosition.x + 1, chunkPosition.y );
	IntVector2 southPosition( chunkPosition.x - 1, chunkPosition.y );
	IntVector2 westPosition( chunkPosition.x, chunkPosition.y + 1 );
	IntVector2 eastPosition( chunkPosition.x, chunkPosition.y - 1 );

	std::map<IntVector2, Chunk*>::const_iterator iterCheck;
	iterCheck = m_chunks.find( northPosition );
	if( iterCheck != m_chunks.end() && iterCheck->second != nullptr )
	{
		blockChunk->m_northChunk = iterCheck->second;
		blockChunk->m_northChunk->m_southChunk = blockChunk;
	}
	else
		blockChunk->m_northChunk = nullptr;

	iterCheck = m_chunks.find( southPosition );
	if( iterCheck != m_chunks.end() && iterCheck->second != nullptr )
	{
		blockChunk->m_southChunk = iterCheck->second;
		blockChunk->m_southChunk->m_northChunk = blockChunk;
	}
	else
		blockChunk->m_southChunk = nullptr;

	iterCheck = m_chunks.find( westPosition );
	if( iterCheck != m_chunks.end() && iterCheck->second != nullptr )
	{
		blockChunk->m_westChunk = iterCheck->second;
		blockChunk->m_westChunk->m_eastChunk = blockChunk;
	}
	else
		blockChunk->m_westChunk = nullptr;

	iterCheck = m_chunks.find( eastPosition );
	if( iterCheck != m_chunks.end() && iterCheck->second != nullptr )
	{
		blockChunk->m_eastChunk = iterCheck->second;
		blockChunk->m_eastChunk->m_westChunk = blockChunk;
	}
	else
		blockChunk->m_eastChunk = nullptr;
}


//-----------------------------------------------------------------------------------------------
void World::SetLightForAddedChunk( Chunk* blockChunk )
{
	int blockCount = NUM_BLOCKS_PER_CHUNK_X_AXIS * NUM_BLOCKS_PER_CHUNK_Y_AXIS * NUM_BLOCKS_PER_CHUNK_Z_AXIS;
	for( int blockIndex = 0; blockIndex < blockCount; ++blockIndex )
	{
		Block* block = &blockChunk->m_blocks[ blockIndex ];
		if( block->m_blockType == GLOWSTONE )
			SetLightForAddedBlock( block );

		if( block->m_blockType == AIR && !block->m_isUnderSky )
			SetLightForAddedBlock( block );
	}
}


//-----------------------------------------------------------------------------------------------
void World::SetLightForEnclosedAirBlocks( Block* block )
{
	if( block->m_blockType != AIR )
		return;

	Chunk* blockChunk = GetChunkBlockIsIn( block );

	if( block->m_isUnderSky && blockChunk->m_skyboxLightValue == 15 )
		return;

	unsigned char highestSurroundingLight = 0;

	Block* northBlock = blockChunk->GetNorthBlock( block );
	if( northBlock != nullptr && northBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* northChunk = GetChunkBlockIsIn( northBlock );
		if( !northChunk->m_updateLight )
			highestSurroundingLight = northBlock->m_internalLightValue;
	}

	Block* southBlock = blockChunk->GetSouthBlock( block );
	if( southBlock != nullptr && southBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* southChunk = GetChunkBlockIsIn( southBlock );
		if( !southChunk->m_updateLight )
			highestSurroundingLight = southBlock->m_internalLightValue;
	}

	Block* westBlock = blockChunk->GetWestBlock( block );
	if( westBlock != nullptr && westBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* westChunk = GetChunkBlockIsIn( westBlock );
		if( !westChunk->m_updateLight )
			highestSurroundingLight = westBlock->m_internalLightValue;
	}

	Block* eastBlock = blockChunk->GetEastBlock( block );
	if( eastBlock != nullptr && eastBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* eastChunk = GetChunkBlockIsIn( eastBlock );
		if( !eastChunk->m_updateLight )
			highestSurroundingLight = eastBlock->m_internalLightValue;
	}

	Block* upBlock = blockChunk->GetUpBlock( block );
	if( upBlock != nullptr && upBlock->m_internalLightValue >= highestSurroundingLight )
		highestSurroundingLight = upBlock->m_internalLightValue;

	Block* downBlock = blockChunk->GetDownBlock( block );
	if( downBlock != nullptr && downBlock->m_internalLightValue >= highestSurroundingLight )
		highestSurroundingLight = downBlock->m_internalLightValue;

	if( highestSurroundingLight == 0 )
	{
		block->m_internalLightValue = 0;
		return;
	}

	if( block->m_isUnderSky && highestSurroundingLight == blockChunk->m_skyboxLightValue )
	{
		block->m_internalLightValue = 4;
		return;
	}

	if( block->m_internalLightValue != ( highestSurroundingLight - 1 ) )
	{
		block->m_internalLightValue = highestSurroundingLight - 1;
		SetLightForAddedBlock( block );
	}
}


//-----------------------------------------------------------------------------------------------
void World::SetLightForRemovedBlock( Block* block )
{
	Chunk* blockChunk = GetChunkBlockIsIn( block );

	if( block->m_isUnderSky && blockChunk->m_skyboxLightValue == 15 )
		return;

	unsigned char highestSurroundingLight = 0;

	Block* northBlock = blockChunk->GetNorthBlock( block );
	if( northBlock != nullptr && northBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* northChunk = GetChunkBlockIsIn( northBlock );
		if( !northChunk->m_updateLight )
			highestSurroundingLight = northBlock->m_internalLightValue;
	}

	Block* southBlock = blockChunk->GetSouthBlock( block );
	if( southBlock != nullptr && southBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* southChunk = GetChunkBlockIsIn( southBlock );
		if( !southChunk->m_updateLight )
			highestSurroundingLight = southBlock->m_internalLightValue;
	}

	Block* westBlock = blockChunk->GetWestBlock( block );
	if( westBlock != nullptr && westBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* westChunk = GetChunkBlockIsIn( westBlock );
		if( !westChunk->m_updateLight )
			highestSurroundingLight = westBlock->m_internalLightValue;
	}

	Block* eastBlock = blockChunk->GetEastBlock( block );
	if( eastBlock != nullptr && eastBlock->m_internalLightValue >= highestSurroundingLight )
	{
		Chunk* eastChunk = GetChunkBlockIsIn( eastBlock );
		if( !eastChunk->m_updateLight )
			highestSurroundingLight = eastBlock->m_internalLightValue;
	}

	Block* upBlock = blockChunk->GetUpBlock( block );
	if( upBlock != nullptr && upBlock->m_internalLightValue >= highestSurroundingLight )
		highestSurroundingLight = upBlock->m_internalLightValue;

	Block* downBlock = blockChunk->GetDownBlock( block );
	if( downBlock != nullptr && downBlock->m_internalLightValue >= highestSurroundingLight )
		highestSurroundingLight = downBlock->m_internalLightValue;

	if( highestSurroundingLight == 0 )
		block->m_internalLightValue = 0;
	else if( block->m_isUnderSky )
		block->m_internalLightValue = highestSurroundingLight;
	else
		block->m_internalLightValue = highestSurroundingLight - 1;
}


//-----------------------------------------------------------------------------------------------
void World::SetLightForAddedBlock( Block* block )
{
	Chunk* blockChunk = GetChunkBlockIsIn( block );

	Block* northBlock = blockChunk->GetNorthBlock( block );
	if( northBlock != nullptr )
	{
		Chunk* northChunk = GetChunkBlockIsIn( northBlock );
		if( !northChunk->m_updateLight )
			SetLightForEnclosedAirBlocks( northBlock );
	}

	Block* southBlock = blockChunk->GetSouthBlock( block );
	if( southBlock != nullptr )
	{
		Chunk* southChunk = GetChunkBlockIsIn( southBlock );
		if( !southChunk->m_updateLight )
			SetLightForEnclosedAirBlocks( southBlock );
	}

	Block* westBlock = blockChunk->GetWestBlock( block );
	if( westBlock != nullptr )
	{
		Chunk* westChunk = GetChunkBlockIsIn( westBlock );
		if( !westChunk->m_updateLight )
			SetLightForEnclosedAirBlocks( westBlock );
	}

	Block* eastBlock = blockChunk->GetEastBlock( block );
	if( eastBlock != nullptr )
	{
		Chunk* eastChunk = GetChunkBlockIsIn( eastBlock );
		if( !eastChunk->m_updateLight )
			SetLightForEnclosedAirBlocks( eastBlock );
	}

	Block* upBlock = blockChunk->GetUpBlock( block );
	if( upBlock != nullptr )
		SetLightForEnclosedAirBlocks( upBlock );

	Block* downBlock = blockChunk->GetDownBlock( block );
	if( downBlock != nullptr )
		SetLightForEnclosedAirBlocks( downBlock );
}


//-----------------------------------------------------------------------------------------------
void World::RemoveBlock( Block* block )
{
	Chunk* blockChunk = GetChunkBlockIsIn( block );
	blockChunk->m_chunkHasBeenEdited = true;

	block->m_blockType = AIR;
	Block* upBlock = blockChunk->GetUpBlock( block );
	if( upBlock != nullptr && upBlock->m_blockType == AIR && upBlock->m_isUnderSky )
	{
		block->m_isUnderSky = true;
		block->m_internalLightValue = blockChunk->m_skyboxLightValue;
		SetBlocksBelowToSeeSky( block );
	}
	
	SetLightForRemovedBlock( block );
	SetLightForAddedBlock( block );
	UpdateSurroundingChunks( blockChunk );
}


//-----------------------------------------------------------------------------------------------
void World::AddBlock( Block* block, unsigned char blockType )
{
	Chunk* blockChunk = GetChunkBlockIsIn( block );
	blockChunk->m_chunkHasBeenEdited = true;

	if( m_traceResult.m_blockFaceImpact == NORTH )
	{
		Block* northBlock = blockChunk->GetNorthBlock( block );
		if( northBlock != nullptr && !CheckIfBoundingBoxIsInBlock( m_playerBox, northBlock ) )
		{
			northBlock->m_blockType = blockType;
			if( blockType == GLOWSTONE )
				northBlock->m_internalLightValue = 12;
			else
				northBlock->m_internalLightValue = 0;
			SetBlocksBelowToNotSeeSky( northBlock );
			SetLightForAddedBlock( northBlock );
			UpdateSurroundingChunks( blockChunk );
			PlayBlockPlaceSound();
		}
	}
	else if( m_traceResult.m_blockFaceImpact == SOUTH )
	{
		Block* southBlock = blockChunk->GetSouthBlock( block );
		if( southBlock != nullptr && !CheckIfBoundingBoxIsInBlock( m_playerBox, southBlock ) )
		{
			southBlock->m_blockType = blockType;
			if( blockType == GLOWSTONE )
				southBlock->m_internalLightValue = 12;
			else
				southBlock->m_internalLightValue = 0;
			SetBlocksBelowToNotSeeSky( southBlock );
			SetLightForAddedBlock( southBlock );
			UpdateSurroundingChunks( blockChunk );
			PlayBlockPlaceSound();
		}
	}
	else if( m_traceResult.m_blockFaceImpact == WEST )
	{
		Block* westBlock = blockChunk->GetWestBlock( block );
		if( westBlock != nullptr && !CheckIfBoundingBoxIsInBlock( m_playerBox, westBlock ) )
		{
			westBlock->m_blockType = blockType;
			if( blockType == GLOWSTONE )
				westBlock->m_internalLightValue = 12;
			else
				westBlock->m_internalLightValue = 0;
			SetBlocksBelowToNotSeeSky( westBlock );
			SetLightForAddedBlock( westBlock );
			UpdateSurroundingChunks( blockChunk );
			PlayBlockPlaceSound();
		}
	}
	else if( m_traceResult.m_blockFaceImpact == EAST )
	{
		Block* eastBlock = blockChunk->GetEastBlock( block );
		if( eastBlock != nullptr && !CheckIfBoundingBoxIsInBlock( m_playerBox, eastBlock ) )
		{
			eastBlock->m_blockType = blockType;
			if( blockType == GLOWSTONE )
				eastBlock->m_internalLightValue = 12;
			else
				eastBlock->m_internalLightValue = 0;
			SetBlocksBelowToNotSeeSky( eastBlock );
			SetLightForAddedBlock( eastBlock );
			UpdateSurroundingChunks( blockChunk );
			PlayBlockPlaceSound();
		}
	}
	else if( m_traceResult.m_blockFaceImpact == UP )
	{
		Block* upBlock = blockChunk->GetUpBlock( block );
		if( upBlock != nullptr && !CheckIfBoundingBoxIsInBlock( m_playerBox, upBlock ) )
		{
			upBlock->m_blockType = blockType;
			if( blockType == GLOWSTONE )
				upBlock->m_internalLightValue = 12;
			else
				upBlock->m_internalLightValue = 0;
			SetBlocksBelowToNotSeeSky( upBlock );
			SetLightForAddedBlock( upBlock );
			UpdateSurroundingChunks( blockChunk );
			PlayBlockPlaceSound();
		}
	}
	else if( m_traceResult.m_blockFaceImpact == DOWN )
	{
		Block* downBlock = blockChunk->GetDownBlock( block );
		if( downBlock != nullptr && !CheckIfBoundingBoxIsInBlock( m_playerBox, downBlock ) )
		{
			downBlock->m_blockType = blockType;
			if( blockType == GLOWSTONE )
				downBlock->m_internalLightValue = 12;
			else
				downBlock->m_internalLightValue = 0;
			SetBlocksBelowToNotSeeSky( downBlock );
			SetLightForAddedBlock( downBlock );
			UpdateSurroundingChunks( blockChunk );
			PlayBlockPlaceSound();
		}
	}
}


//-----------------------------------------------------------------------------------------------
void World::CheckForBlockRemoval()
{
	if( m_traceResult.m_blockFirstHit != nullptr && m_mouse.IsLeftButtonClicked() )
	{
		Block* block = m_traceResult.m_blockFirstHit;
		PlayBlockDigSound( block );
		RemoveBlock( block );
	}
}


//-----------------------------------------------------------------------------------------------
void World::CheckForBlockAdd()
{
	Block* block = m_traceResult.m_blockFirstHit;

	if( block != nullptr && m_mouse.IsRightButtonClicked() )
	{
		AddBlock( block, DIRT );
	}
	else if( block != nullptr && m_keyboard.IsKeyPressedDown( KEY_1 ) && !m_keyboard.WasKeyPressedDown( KEY_1 ) )
	{
		AddBlock( block, GLOWSTONE );
	}
}


//-----------------------------------------------------------------------------------------------
void World::RemoveChunk( Chunk* blockChunk )
{
	if( blockChunk->m_chunkHasBeenEdited )
		blockChunk->SaveChunkToFile();

	if( blockChunk->m_northChunk != nullptr )
		blockChunk->m_northChunk->m_southChunk = nullptr;
	if( blockChunk->m_southChunk != nullptr )
		blockChunk->m_southChunk->m_northChunk = nullptr;
	if( blockChunk->m_westChunk != nullptr )
		blockChunk->m_westChunk->m_eastChunk = nullptr;
	if( blockChunk->m_eastChunk != nullptr )
		blockChunk->m_eastChunk->m_westChunk = nullptr;

	delete blockChunk;
}


//-----------------------------------------------------------------------------------------------
void World::AddChunk( const IntVector2& chunkPosition )
{
	Texture* texAtlas = Texture::GetTextureByName( "Data/Images/SimpleMinerAtlas.png" );
	Chunk* blockChunk = new Chunk( Vector2( (float) ( chunkPosition.x * NUM_BLOCKS_PER_CHUNK_X_AXIS ), (float) ( chunkPosition.y * NUM_BLOCKS_PER_CHUNK_Y_AXIS ) ) );
	blockChunk->m_texAtlas = texAtlas;
	if( m_timeOfDay < DAY_TRANSITION_MIDDLE_TIME || m_timeOfDay > NIGHT_TRANSITION_MIDDLE_TIME )
		blockChunk->m_skyboxLightValue = NIGHT_SKYBOX_LIGHT_VALUE;
	else
		blockChunk->m_skyboxLightValue = 15;

	if( blockChunk->CheckIfFileExists() )
		blockChunk->LoadChunkFromFile();
	else
		blockChunk->SetBlockChunk();

	m_chunks[ chunkPosition ] = blockChunk;

	SetSurroundingChunks( blockChunk );
	SetLightForAddedChunk( blockChunk );
	UpdateSurroundingChunks( blockChunk );
}


//-----------------------------------------------------------------------------------------------
void World::CheckForChunkRemoval()
{
	std::vector<IntVector2> chunksToRemove;
	std::map<IntVector2, Chunk*>::iterator iter;
	int chunksRemoved = 0;

	for( iter = m_chunks.begin(); iter != m_chunks.end(); ++iter )
	{
		if( chunksRemoved == NUM_CHUNKS_TO_REMOVE_PER_FRAME )
			break;

		if( iter->second == nullptr )
		{
			chunksToRemove.push_back( iter->first );
			continue;
		}

		Vector2 cameraPosIn2D( m_camera.m_position.x, m_camera.m_position.y );

		Chunk* blockChunk = iter->second;
		Vector2 chunkCenterPosition = blockChunk->m_position + Vector2( NUM_BLOCKS_PER_CHUNK_X_AXIS * 0.5f, NUM_BLOCKS_PER_CHUNK_Y_AXIS * 0.5f );
		float removeRadius = REMOVE_RADIUS_LENGTH_IN_CHUNKS * NUM_BLOCKS_PER_CHUNK_X_AXIS;
		if( GetSquaredDistanceBetweenPoints2D( cameraPosIn2D, chunkCenterPosition ) > ( removeRadius * removeRadius ) )
		{
			chunksToRemove.push_back( iter->first );
			RemoveChunk( blockChunk );
			++chunksRemoved;
		}
	}

	for( unsigned int chunkIndex = 0; chunkIndex < chunksToRemove.size(); ++chunkIndex )
	{
		IntVector2 chunkPosition = chunksToRemove[ chunkIndex ];
		m_chunks.erase( chunkPosition );
	}
}


//-----------------------------------------------------------------------------------------------
void World::CheckForChunkAdd()
{
	Vector3 camPosInChunkCoords = m_camera.m_position * ONE_OVER_SIXTEEN;
	camPosInChunkCoords.x = floor( camPosInChunkCoords.x );
	camPosInChunkCoords.y = floor( camPosInChunkCoords.y );
	int chunkCountMinX = (int) camPosInChunkCoords.x - VIEW_RADIUS_LENGTH_IN_CHUNKS;
	int chunkCountMaxX = (int) camPosInChunkCoords.x + VIEW_RADIUS_LENGTH_IN_CHUNKS;
	int chunkCountMinY = (int) camPosInChunkCoords.y - VIEW_RADIUS_LENGTH_IN_CHUNKS;
	int chunkCountMaxY = (int) camPosInChunkCoords.y + VIEW_RADIUS_LENGTH_IN_CHUNKS;
	int chunksAdded = 0;
	std::map<IntVector2, Chunk*>::iterator iter;

	for( int chunkIndexY = chunkCountMinY; chunkIndexY < chunkCountMaxY; ++chunkIndexY )
	{
		for( int chunkIndexX = chunkCountMinX; chunkIndexX < chunkCountMaxX; ++chunkIndexX )
		{
			if( chunksAdded == NUM_CHUNKS_TO_ADD_PER_FRAME )
				return;

			IntVector2 chunkPosition( chunkIndexX, chunkIndexY );
			iter = m_chunks.find( chunkPosition );
			if( iter != m_chunks.end() && iter->second != nullptr )
				continue;

			Vector2 cameraPosIn2D( m_camera.m_position.x, m_camera.m_position.y );
			Vector2 chunkCenterPosition( (float) chunkIndexX * NUM_BLOCKS_PER_CHUNK_X_AXIS, (float) chunkIndexY * NUM_BLOCKS_PER_CHUNK_Y_AXIS );
			chunkCenterPosition += Vector2( NUM_BLOCKS_PER_CHUNK_X_AXIS * 0.5f, NUM_BLOCKS_PER_CHUNK_Y_AXIS * 0.5f );
			float addRadius = VIEW_RADIUS_LENGTH_IN_CHUNKS * NUM_BLOCKS_PER_CHUNK_X_AXIS;;
			if( GetSquaredDistanceBetweenPoints2D( cameraPosIn2D, chunkCenterPosition ) < ( addRadius * addRadius ) )
			{
				AddChunk( chunkPosition );
				++chunksAdded;

				if( !m_initialized )
					++m_numChunksInitialized;
			}
		}
	}

	if( chunksAdded == 0 )
		m_initialized = true;
}


//-----------------------------------------------------------------------------------------------
void World::SetBlocksBelowToSeeSky( Block* block )
{
	Chunk* blockChunk = GetChunkBlockIsIn( block );

	Block* downBlock = blockChunk->GetDownBlock( block );
	if( downBlock != nullptr && downBlock->m_blockType == AIR && !downBlock->m_isUnderSky )
	{
		downBlock->m_isUnderSky = true;
		downBlock->m_internalLightValue = GetChunkBlockIsIn( downBlock )->m_skyboxLightValue;
		SetBlocksBelowToSeeSky( downBlock );
	}

	SetLightForAddedBlock( block );
}


//-----------------------------------------------------------------------------------------------
void World::SetBlocksBelowToNotSeeSky( Block* block )
{
	Chunk* blockChunk = GetChunkBlockIsIn( block );

	Block* downBlock = blockChunk->GetDownBlock( block );
	if( downBlock != nullptr && downBlock->m_isUnderSky )
	{
		downBlock->m_isUnderSky = false;
		SetBlocksBelowToNotSeeSky( downBlock );
	}
}


//-----------------------------------------------------------------------------------------------
void World::SetFaceHighlighting()
{
	if( !m_initialized )
		return;

	Block* block = m_traceResult.m_blockFirstHit;
	Directions faceDirection = m_traceResult.m_blockFaceImpact;

	if( faceDirection == NORTH )
	{
		block->m_highlightFaceFlag = CULL_POSITIVE_X_FACE;
	}
	else if( faceDirection == SOUTH )
	{
		block->m_highlightFaceFlag = CULL_NEGATIVE_X_FACE;
	}
	else if( faceDirection == WEST )
	{
		block->m_highlightFaceFlag = CULL_POSITIVE_Y_FACE;
	}
	else if( faceDirection == EAST )
	{
		block->m_highlightFaceFlag = CULL_NEGATIVE_Y_FACE;
	}
	else if( faceDirection == UP )
	{
		block->m_highlightFaceFlag = CULL_POSITIVE_Z_FACE;
	}
	else if( faceDirection == DOWN )
	{
		block->m_highlightFaceFlag = CULL_NEGATIVE_Z_FACE;
	}
}


//-----------------------------------------------------------------------------------------------
void World::CheckForSkyboxLightChange()
{
	if( !m_initialized )
		return;

	if( !m_changedToNight && m_timeOfDay > NIGHT_TRANSITION_MIDDLE_TIME )
	{
		UpdateSkyboxLightValue();
		m_changedToNight = true;
	}
	else if( !m_changedToDay && m_timeOfDay > DAY_TRANSITION_MIDDLE_TIME )
	{
		UpdateSkyboxLightValue();
		m_changedToDay = true;
	}
}


//-----------------------------------------------------------------------------------------------
std::set<Block*> World::GetBlocksBoundingBoxIsIn( const AABB3& boundingBox )
{
	std::set<Block*> blocks;
	Vector3 centerPos = boundingBox.m_centerPosition;

	Vector3 topNorthWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topNorthEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topSouthWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topSouthEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 middleNorthWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z );
	Vector3 middleNorthEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z );
	Vector3 middleSouthWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z );
	Vector3 middleSouthEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z );
	Vector3 bottomNorthWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomNorthEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomSouthWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomSouthEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );

	blocks.insert( GetBlock( topNorthWestCorner ) );
	blocks.insert( GetBlock( topNorthEastCorner ) );
	blocks.insert( GetBlock( topSouthWestCorner ) );
	blocks.insert( GetBlock( topSouthEastCorner ) );
	blocks.insert( GetBlock( middleNorthWestCorner ) );
	blocks.insert( GetBlock( middleNorthEastCorner ) );
	blocks.insert( GetBlock( middleSouthWestCorner ) );
	blocks.insert( GetBlock( middleSouthEastCorner ) );
	blocks.insert( GetBlock( bottomNorthWestCorner ) );
	blocks.insert( GetBlock( bottomNorthEastCorner ) );
	blocks.insert( GetBlock( bottomSouthWestCorner ) );
	blocks.insert( GetBlock( bottomSouthEastCorner ) );

	return blocks;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForChunkCull( const Vector2& frustum, const Vector2& chunkPosition )
{
	if( m_camera.m_position.x >= chunkPosition.x && m_camera.m_position.x < ( chunkPosition.x + NUM_BLOCKS_PER_CHUNK_X_AXIS ) )
	{
		if( m_camera.m_position.y >= chunkPosition.y && m_camera.m_position.y < ( chunkPosition.y + NUM_BLOCKS_PER_CHUNK_Y_AXIS ) )
			return false;
	}

	Vector2 cameraPushedBack;
	cameraPushedBack.x = m_camera.m_position.x - ( 2.f * NUM_BLOCKS_PER_CHUNK_X_AXIS * cos( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) ) );
	cameraPushedBack.y = m_camera.m_position.y - ( 2.f * NUM_BLOCKS_PER_CHUNK_Y_AXIS * sin( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) ) );

	Vector2 camToChunk = chunkPosition - cameraPushedBack;
	cameraPushedBack.Normalize();

	float dotProd = DotProduct( frustum, camToChunk );
	if( dotProd < 0.f )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckIfBoundingBoxIsOnlyInAir( const AABB3& boundingBox )
{
	std::set<Block*> blocks = GetBlocksBoundingBoxIsIn( boundingBox );

	std::set<Block*>::iterator iter;
	for( iter = blocks.begin(); iter != blocks.end(); ++iter )
	{
		Block* checkBlock = *iter;
		if( checkBlock != nullptr && checkBlock->m_blockType != AIR )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckIfBoundingBoxIsInBlock( const AABB3& boundingBox, Block* block )
{
	std::set<Block*> blocks = GetBlocksBoundingBoxIsIn( boundingBox );

	std::set<Block*>::iterator iter;
	for( iter = blocks.begin(); iter != blocks.end(); ++iter )
	{
		Block* checkBlock = *iter;
		if( checkBlock == block )
			return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckIfBoundingBoxIsOnGround( const AABB3& boundingBox )
{
	AABB3 adjustedBox = boundingBox;
	adjustedBox.m_centerPosition.z -= ( MARGIN_OF_ERROR * 2.f );
	return CheckForBottomFaceCollision( adjustedBox );
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForNorthFaceCollision( const AABB3& boundingBox )
{
	Vector3 centerPos = boundingBox.m_centerPosition;
	Vector3 topWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 bottomWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );

	Block* topWestBlock = GetBlock( topWestCorner );
	Block* topEastBlock = GetBlock( topEastCorner );
	Block* bottomWestBlock = GetBlock( bottomWestCorner );
	Block* bottomEastBlock = GetBlock( bottomEastCorner );

	if( topWestBlock != nullptr && topWestBlock->m_blockType != AIR )
		return true;
	if( topEastBlock != nullptr && topEastBlock->m_blockType != AIR )
		return true;
	if( bottomWestBlock != nullptr && bottomWestBlock->m_blockType != AIR )
		return true;
	if( bottomEastBlock != nullptr && bottomEastBlock->m_blockType != AIR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForSouthFaceCollision( const AABB3& boundingBox )
{
	Vector3 centerPos = boundingBox.m_centerPosition;
	Vector3 topWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 bottomWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );

	Block* topWestBlock = GetBlock( topWestCorner );
	Block* topEastBlock = GetBlock( topEastCorner );
	Block* bottomWestBlock = GetBlock( bottomWestCorner );
	Block* bottomEastBlock = GetBlock( bottomEastCorner );

	if( topWestBlock != nullptr && topWestBlock->m_blockType != AIR )
		return true;
	if( topEastBlock != nullptr && topEastBlock->m_blockType != AIR )
		return true;
	if( bottomWestBlock != nullptr && bottomWestBlock->m_blockType != AIR )
		return true;
	if( bottomEastBlock != nullptr && bottomEastBlock->m_blockType != AIR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForWestFaceCollision( const AABB3& boundingBox )
{
	Vector3 centerPos = boundingBox.m_centerPosition;
	Vector3 topNorthCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topSouthCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 bottomNorthCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomSouthCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );

	Block* topNorthBlock = GetBlock( topNorthCorner );
	Block* topSouthBlock = GetBlock( topSouthCorner );
	Block* bottomNorthBlock = GetBlock( bottomNorthCorner );
	Block* bottomSouthBlock = GetBlock( bottomSouthCorner );

	if( topNorthBlock != nullptr && topNorthBlock->m_blockType != AIR )
		return true;
	if( topSouthBlock != nullptr && topSouthBlock->m_blockType != AIR )
		return true;
	if( bottomNorthBlock != nullptr && bottomNorthBlock->m_blockType != AIR )
		return true;
	if( bottomSouthBlock != nullptr && bottomSouthBlock->m_blockType != AIR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForEastFaceCollision( const AABB3& boundingBox )
{
	Vector3 centerPos = boundingBox.m_centerPosition;
	Vector3 topNorthCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 topSouthCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 bottomNorthCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 bottomSouthCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );

	Block* topNorthBlock = GetBlock( topNorthCorner );
	Block* topSouthBlock = GetBlock( topSouthCorner );
	Block* bottomNorthBlock = GetBlock( bottomNorthCorner );
	Block* bottomSouthBlock = GetBlock( bottomSouthCorner );

	if( topNorthBlock != nullptr && topNorthBlock->m_blockType != AIR )
		return true;
	if( topSouthBlock != nullptr && topSouthBlock->m_blockType != AIR )
		return true;
	if( bottomNorthBlock != nullptr && bottomNorthBlock->m_blockType != AIR )
		return true;
	if( bottomSouthBlock != nullptr && bottomSouthBlock->m_blockType != AIR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForTopFaceCollision( const AABB3& boundingBox )
{
	Vector3 centerPos = boundingBox.m_centerPosition;
	Vector3 northWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 northEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 southWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );
	Vector3 southEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z + boundingBox.m_zAxisHalfLength );

	Block* northWestBlock = GetBlock( northWestCorner );
	Block* northEastBlock = GetBlock( northEastCorner );
	Block* southWestBlock = GetBlock( southWestCorner );
	Block* southEastBlock = GetBlock( southEastCorner );

	if( northWestBlock != nullptr && northWestBlock->m_blockType != AIR )
		return true;
	if( northEastBlock != nullptr && northEastBlock->m_blockType != AIR )
		return true;
	if( southWestBlock != nullptr && southWestBlock->m_blockType != AIR )
		return true;
	if( southEastBlock != nullptr && southEastBlock->m_blockType != AIR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool World::CheckForBottomFaceCollision( const AABB3& boundingBox )
{
	Vector3 centerPos = boundingBox.m_centerPosition;
	Vector3 northWestCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 northEastCorner( centerPos.x + boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 southWestCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y + boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );
	Vector3 southEastCorner( centerPos.x - boundingBox.m_xAxisHalfLength, centerPos.y - boundingBox.m_yAxisHalfLength, centerPos.z - boundingBox.m_zAxisHalfLength );

	Block* northWestBlock = GetBlock( northWestCorner );
	Block* northEastBlock = GetBlock( northEastCorner );
	Block* southWestBlock = GetBlock( southWestCorner );
	Block* southEastBlock = GetBlock( southEastCorner );

	if( northWestBlock != nullptr && northWestBlock->m_blockType != AIR )
		return true;
	if( northEastBlock != nullptr && northEastBlock->m_blockType != AIR )
		return true;
	if( southWestBlock != nullptr && southWestBlock->m_blockType != AIR )
		return true;
	if( southEastBlock != nullptr && southEastBlock->m_blockType != AIR )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
void World::AdjustBoundingBoxForNorthSouthCollision( AABB3& boundingBox )
{
	Vector3 playerPositionDifference = boundingBox.m_centerPosition - m_playerPrevPosition;

	if( playerPositionDifference.x > 0.f && CheckForNorthFaceCollision( boundingBox ) )
	{
		boundingBox.m_centerPosition.x -= boundingBox.m_xAxisHalfLength;
		boundingBox.m_centerPosition.x = ceil( boundingBox.m_centerPosition.x ) - boundingBox.m_xAxisHalfLength - MARGIN_OF_ERROR;
	}
	else if( playerPositionDifference.x < 0.f && CheckForSouthFaceCollision( boundingBox ) )
	{
		boundingBox.m_centerPosition.x += boundingBox.m_xAxisHalfLength;
		boundingBox.m_centerPosition.x = floor( boundingBox.m_centerPosition.x ) + boundingBox.m_xAxisHalfLength + MARGIN_OF_ERROR;
	}
}


//-----------------------------------------------------------------------------------------------
void World::AdjustBoundingBoxForEastWestCollision( AABB3& boundingBox )
{
	Vector3 playerPositionDifference = boundingBox.m_centerPosition - m_playerPrevPosition;

	if( playerPositionDifference.y > 0.f && CheckForWestFaceCollision( boundingBox ) )
	{
		boundingBox.m_centerPosition.y -= boundingBox.m_yAxisHalfLength;
		boundingBox.m_centerPosition.y = ceil( boundingBox.m_centerPosition.y ) - boundingBox.m_yAxisHalfLength - MARGIN_OF_ERROR;
	}
	else if( playerPositionDifference.y < 0.f && CheckForEastFaceCollision( boundingBox ) )
	{
		boundingBox.m_centerPosition.y += boundingBox.m_yAxisHalfLength;
		boundingBox.m_centerPosition.y = floor( boundingBox.m_centerPosition.y ) + boundingBox.m_yAxisHalfLength + MARGIN_OF_ERROR;
	}
}


//-----------------------------------------------------------------------------------------------
void World::AdjustBoundingBoxForUpDownCollision( AABB3& boundingBox )
{
	Vector3 playerPositionDifference = boundingBox.m_centerPosition - m_playerPrevPosition;

	if( playerPositionDifference.z > 0.f && CheckForTopFaceCollision( boundingBox ) )
	{
		boundingBox.m_centerPosition.z += boundingBox.m_zAxisHalfLength;
		boundingBox.m_centerPosition.z = floor( boundingBox.m_centerPosition.z ) - boundingBox.m_zAxisHalfLength - MARGIN_OF_ERROR;
		m_fallSpeed = 0.f;
	}
	else if( playerPositionDifference.z < 0.f && CheckForBottomFaceCollision( boundingBox ) )
	{
		boundingBox.m_centerPosition.z -= boundingBox.m_zAxisHalfLength;
		boundingBox.m_centerPosition.z = ceil( boundingBox.m_centerPosition.z ) + boundingBox.m_zAxisHalfLength + MARGIN_OF_ERROR;
		m_fallSpeed = 0.f;
	}
}


//-----------------------------------------------------------------------------------------------
void World::AdjustBoundingBoxForCollision( AABB3& boundingBox )
{
	if( !m_initialized )
		return;

	if( m_movementMode == NO_CLIP )
		return;
	
	Vector3 playerPositionDifference = boundingBox.m_centerPosition - m_playerPrevPosition;

	if( abs( playerPositionDifference.x ) < abs( playerPositionDifference.y ) )
	{
		AdjustBoundingBoxForNorthSouthCollision( boundingBox );
		AdjustBoundingBoxForEastWestCollision( boundingBox );
	}
	else
	{
		AdjustBoundingBoxForEastWestCollision( boundingBox );
		AdjustBoundingBoxForNorthSouthCollision( boundingBox );
	}
	
	AdjustBoundingBoxForUpDownCollision( boundingBox );

	m_isOnGround = CheckIfBoundingBoxIsOnGround( boundingBox );
}


//-----------------------------------------------------------------------------------------------
void World::PlayWalkSound()
{
	if( m_movementMode == NO_CLIP )
		return;

	bool isPlaying;
	m_walkChannel->isPlaying( &isPlaying );
	if( !isPlaying )
	{
		Vector3 blockPoint = m_playerBox.m_centerPosition;
		blockPoint.z -= ( m_playerBox.m_zAxisHalfLength + ( MARGIN_OF_ERROR * 2.f ) );
		Block* block = GetBlock( blockPoint );
		if( block == nullptr )
			return;
		if( block->m_blockType == AIR )
			return;

		if( block->m_blockType == DIRT )
		{
			static int lastDirtIndex = -1;
			int dirtIndex = lastDirtIndex;
			while( dirtIndex == lastDirtIndex )
				dirtIndex = rand() % m_walkDirtSounds.size();

			lastDirtIndex = dirtIndex;
			m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_walkDirtSounds[ dirtIndex ], false, &m_walkChannel );
		}
		else
		{
			static int lastStoneIndex = -1;
			int stoneIndex = lastStoneIndex;
			while( stoneIndex == lastStoneIndex )
				stoneIndex = rand() % m_walkStoneSounds.size();

			lastStoneIndex = stoneIndex;
			m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_walkStoneSounds[ stoneIndex ], false, &m_walkChannel );
		}
	}
}


//-----------------------------------------------------------------------------------------------
void World::PlayBlockDigSound( Block* block )
{
	if( block == nullptr )
		return;
	if( block->m_blockType == AIR )
		return;

	if( block->m_blockType == DIRT )
	{
		int dirtIndex = rand() % m_digDirtSounds.size();
		m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_digDirtSounds[ dirtIndex ], false, &m_digChannel );
	}
	else
	{
		int stoneIndex = rand() % m_digStoneSounds.size();
		m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_digStoneSounds[ stoneIndex ], false, &m_digChannel );
	}
}


//-----------------------------------------------------------------------------------------------
void World::PlayBlockPlaceSound()
{
	m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_placeSound, false, &m_digChannel );
}


//-----------------------------------------------------------------------------------------------
void World::UpdateBackgroundMusic( float deltaSeconds )
{
	if( !m_initialized )
		return;

	bool isPlaying;
	m_musicChannel->isPlaying( &isPlaying );
	if( !isPlaying )
	{
		m_secondsSinceSongEnd += deltaSeconds;
		if( m_secondsSinceSongEnd > SECONDS_BETWEEN_SONGS )
		{
			m_secondsSinceSongEnd = 0;

			static int lastSongIndex = -1;
			int songIndex = lastSongIndex;
			while( songIndex == lastSongIndex )
				songIndex = rand() % m_musicSounds.size();

			lastSongIndex = songIndex;
			m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_musicSounds[ songIndex ], false, &m_musicChannel );
		}
	}
}


//-----------------------------------------------------------------------------------------------
void World::UpdateSkyboxLightValue()
{
	std::map<IntVector2, Chunk*>::iterator iter;
	for( iter = m_chunks.begin(); iter != m_chunks.end(); ++iter )
	{
		Chunk* blockChunk = iter->second;

		if( m_timeOfDay > NIGHT_TRANSITION_MIDDLE_TIME )
			blockChunk->m_skyboxLightValue = NIGHT_SKYBOX_LIGHT_VALUE;
		else
			blockChunk->m_skyboxLightValue = 15;

		blockChunk->m_updateLight = true;
	}
}


//-----------------------------------------------------------------------------------------------
void World::UpdateSurroundingChunks( Chunk* blockChunk )
{
	blockChunk->m_updateBuffer = true;

	if( blockChunk->m_northChunk != nullptr )
		blockChunk->m_northChunk->m_updateBuffer = true;

	if( blockChunk->m_southChunk != nullptr )
		blockChunk->m_southChunk->m_updateBuffer = true;

	if( blockChunk->m_westChunk != nullptr )
		blockChunk->m_westChunk->m_updateBuffer = true;

	if( blockChunk->m_eastChunk != nullptr )
		blockChunk->m_eastChunk->m_updateBuffer = true;
}


//-----------------------------------------------------------------------------------------------
void World::UpdateChunks()
{
	int updatedChunks = 0;
	std::map<IntVector2, Chunk*>::iterator iter;

	for( iter = m_chunks.begin(); iter != m_chunks.end(); ++iter )
	{
		if( updatedChunks > NUM_CHUNKS_TO_UPDATE_PER_FRAME )
			return;

		Chunk* blockChunk = iter->second;
		if( blockChunk->m_updateBuffer )
		{
			UpdateChunkBuffers( blockChunk );
			blockChunk->m_updateBuffer = false;
			++updatedChunks;
		}
		if( blockChunk->m_updateLight )
		{
			blockChunk->m_updateLight = false;
			blockChunk->m_updateBuffer = true;
			blockChunk->SetBlocksInternalLightValue();
			SetLightForAddedChunk( blockChunk );

			if( blockChunk->m_eastChunk != nullptr )
			{
				blockChunk->m_eastChunk->m_updateLight = false;
				blockChunk->m_eastChunk->m_updateBuffer = true;
				blockChunk->m_eastChunk->SetBlocksInternalLightValue();
				SetLightForAddedChunk( blockChunk->m_eastChunk );
			}

			++updatedChunks;
		}
	}
}


//-----------------------------------------------------------------------------------------------
void World::UpdateFromInput( float deltaSeconds )
{
	if( !m_initialized )
		return;

	if( m_keyboard.IsKeyPressedDown( KEY_J ) && CheckIfBoundingBoxIsOnlyInAir( m_playerBox ) )
	{
		m_movementMode = WALKING;
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_J ) && !m_keyboard.WasKeyPressedDown( KEY_J ) )
	{
		m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_stayNoClipSound, false, &m_digChannel );
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_K ) && CheckIfBoundingBoxIsOnlyInAir( m_playerBox ) )
	{
		m_movementMode = FLYING;
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_K ) && !m_keyboard.WasKeyPressedDown( KEY_K ) )
	{
		m_audioSystem->playSound( FMOD_CHANNEL_FREE, m_stayNoClipSound, false, &m_digChannel );
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_L ) )
	{
		m_movementMode = NO_CLIP;
	}

	float moveHorizontalSpeedBlocksPerSecond = 0;
	if( m_movementMode == WALKING )
	{
		if( m_isOnGround )
			moveHorizontalSpeedBlocksPerSecond = WALKING_MOVE_SPEED_BLOCKS_PER_SECOND;
		else
			moveHorizontalSpeedBlocksPerSecond = WALKING_MOVE_SPEED_OFF_GROUND_BLOCKS_PER_SECOND;
	}
	else if( m_movementMode == FLYING )
		moveHorizontalSpeedBlocksPerSecond = FLYING_MOVE_SPEED_BLOCKS_PER_SECOND;
	else if( m_movementMode == NO_CLIP )
		moveHorizontalSpeedBlocksPerSecond = NO_CLIP_MOVE_SPEED_BLOCKS_PER_SECOND;
	
	float rotationRadiansPerSecond = ConvertDegreesToRadians( ROTATION_DEGREES_PER_SECOND );
	float yawRadians = ConvertDegreesToRadians( m_camera.m_orientation.yaw );
	static bool cameraForward = false;
	static bool cameraBack = false;
	static bool cameraLeft = false;
	static bool cameraRight = false;

	Vector3 cameraForwardXY( cos( yawRadians ), sin( yawRadians ), 0.f );
	Vector3 cameraLeftXY( -cameraForwardXY.y, cameraForwardXY.x, 0.f );

	Vector3 cameraMoveVector( 0.f, 0.f, 0.f );
	EulerAngles cameraRotation( 0.f, 0.f, 0.f );

	Vector2 currentMousePosition = m_mouse.GetCursorPosition();

	m_mouse.SetCursorPosition( Vector2( ( m_size.x * 0.5f ), ( m_size.y * 0.5f ) ) );

	if( m_keyboard.IsKeyPressedDown( KEY_SPACE ) && !m_keyboard.WasKeyPressedDown( KEY_SPACE ) && m_isOnGround )
	{
		m_fallSpeed = -JUMP_SPEED_BLOCKS_PER_SECOND;
	}

	if( m_keyboard.IsKeyPressedDown( KEY_W ) )
	{
		cameraMoveVector += cameraForwardXY;
		cameraForward = true;
		if( !m_isMoving )
			m_secondsSinceMoveStart = 0.f;
		m_isMoving = true;
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_S ) )
	{
		cameraMoveVector -= cameraForwardXY;
		cameraBack = true;
		if( !m_isMoving )
			m_secondsSinceMoveStart = 0.f;
		m_isMoving = true;
	}

	if( m_keyboard.IsKeyPressedDown( KEY_A ) )
	{
		cameraMoveVector += cameraLeftXY;
		cameraLeft = true;
		if( !m_isMoving )
			m_secondsSinceMoveStart = 0.f;
		m_isMoving = true;
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_D ) )
	{
		cameraMoveVector -= cameraLeftXY;
		cameraRight = true;
		if( !m_isMoving )
			m_secondsSinceMoveStart = 0.f;
		m_isMoving = true;
	}

	if( m_isMoving && m_isOnGround )
	{
		PlayWalkSound();
	}

	if( m_isMoving && !m_keyboard.IsKeyPressedDown( KEY_W ) && !m_keyboard.IsKeyPressedDown( KEY_S ) && !m_keyboard.IsKeyPressedDown( KEY_A ) && !m_keyboard.IsKeyPressedDown( KEY_D ) )
	{
		m_secondsSinceMoveStop = 0.f;
		m_isMoving = false;
	}

	if( m_keyboard.IsKeyPressedDown( KEY_E ) && m_movementMode != WALKING )
	{
		cameraMoveVector.z += 1.f;
	}
	else if( m_keyboard.IsKeyPressedDown( KEY_Q ) && m_movementMode != WALKING )
	{
		if( m_movementMode == NO_CLIP || !m_isOnGround )
			cameraMoveVector.z -= 1.f;
	}

	if( m_isMoving && m_secondsSinceMoveStart < TIME_TO_FULL_SPEED_FULL_STOP )
	{
		moveHorizontalSpeedBlocksPerSecond *= m_secondsSinceMoveStart / TIME_TO_FULL_SPEED_FULL_STOP;
		m_secondsSinceMoveStart += deltaSeconds;
	}

	bool updatePrevCam = true;

	if( !m_isMoving && m_secondsSinceMoveStop < TIME_TO_FULL_SPEED_FULL_STOP )
	{
		if( cameraForward )
			cameraMoveVector += m_prevCameraForwardXY;
		else if( cameraBack )
			cameraMoveVector -= m_prevCameraForwardXY;

		if( cameraLeft )
			cameraMoveVector += m_prevcameraLeftXY;
		else if( cameraRight )
			cameraMoveVector -= m_prevcameraLeftXY;

		updatePrevCam = false;

		moveHorizontalSpeedBlocksPerSecond *= 1.f - ( m_secondsSinceMoveStop / TIME_TO_FULL_SPEED_FULL_STOP );
		m_secondsSinceMoveStop += deltaSeconds;

		if( m_secondsSinceMoveStop >= TIME_TO_FULL_SPEED_FULL_STOP )
		{
			cameraForward = false;
			cameraBack = false;
			cameraLeft = false;
			cameraRight = false;
		}
	}

	if( m_movementMode == WALKING )
	{
		cameraMoveVector.z = -m_fallSpeed;
	}

	cameraRotation.yaw += ( m_size.x * 0.5f ) - currentMousePosition.x;
	cameraRotation.pitch += currentMousePosition.y - ( m_size.y * 0.5f );

	cameraMoveVector.x *= ( moveHorizontalSpeedBlocksPerSecond * deltaSeconds );
	cameraMoveVector.y *= ( moveHorizontalSpeedBlocksPerSecond * deltaSeconds );
	cameraMoveVector.z *= ( GRAVITY_ACCELERATION_BLOCKS_PER_SECOND * deltaSeconds );
	cameraRotation *= ( rotationRadiansPerSecond * deltaSeconds );
	m_playerBox.m_centerPosition += cameraMoveVector;
	m_camera.m_orientation += cameraRotation;

	if( m_camera.m_orientation.pitch > 90.f )
	{
		m_camera.m_orientation.pitch = 90.f;
	}
	else if( m_camera.m_orientation.pitch < -90.f )
	{
		m_camera.m_orientation.pitch = -90.f;
	}

	if( updatePrevCam )
	{
		m_prevCameraForwardXY = cameraForwardXY;
		m_prevcameraLeftXY = cameraLeftXY;
	}
}


//-----------------------------------------------------------------------------------------------
void World::UpdateCameraPosition()
{
	m_camera.m_position = m_playerBox.m_centerPosition;
	m_camera.m_position.z += CAMERA_DISTANCE_FROM_BOX_BOTTOM - m_playerBox.m_zAxisHalfLength;
}


//-----------------------------------------------------------------------------------------------
void World::UpdateRayCasting()
{
	if( !m_initialized )
		return;

	Vector3 directionalVector;
	directionalVector.x = cos( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) ) * cos( ConvertDegreesToRadians( m_camera.m_orientation.pitch ) );
	directionalVector.y = sin( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) ) * cos( ConvertDegreesToRadians( m_camera.m_orientation.pitch ) );
	directionalVector.z = -sin( ConvertDegreesToRadians( m_camera.m_orientation.pitch ) );
	directionalVector *= RAY_CAST_LENGTH_IN_BLOCKS;

	Vector3 endPoint =  m_camera.m_position + directionalVector;
	m_traceResult = CastRay( m_camera.m_position, endPoint );
}


//-----------------------------------------------------------------------------------------------
void World::UpdateChunkBuffers( Chunk* blockChunk )
{
	if( blockChunk == nullptr )
		return;

	blockChunk->m_blockVertices.clear();
	blockChunk->SetSideCulling();
}


//-----------------------------------------------------------------------------------------------
void World::UpdateTimeOfDay( float deltaSeconds )
{
	if( !m_initialized )
		return;

	m_timeOfDay += TWENTYFOUR_OVER_ROTATION_SECONDS * deltaSeconds;
	if( m_timeOfDay > 24.f )
	{
		m_timeOfDay -= 24.f;
		m_changedToDay = false;
		m_changedToNight = false;
	}

	m_moonAngleRadians = m_timeOfDay * ONE_OVER_TWENTYFOUR * TWO_PI;
	m_sunAngleRadians = m_moonAngleRadians - pi;
}


//-----------------------------------------------------------------------------------------------
void World::Update()
{
	float deltaSeconds = static_cast< float >( FRAME_TIME_SECONDS );

	if( m_initialized && !m_isOnGround && m_movementMode == WALKING )
		m_fallSpeed += GRAVITY_ACCELERATION_BLOCKS_PER_SECOND * deltaSeconds;
	else
		m_fallSpeed = 0.f;

	UpdateFromInput( deltaSeconds );
	UpdateTimeOfDay( deltaSeconds );
	UpdateBackgroundMusic( deltaSeconds );
	CheckForSkyboxLightChange();
	UpdateRayCasting();
	SetFaceHighlighting();
	CheckForBlockRemoval();
	CheckForBlockAdd();
	CheckForChunkRemoval();
	CheckForChunkAdd();
	UpdateChunks();
	AdjustBoundingBoxForCollision( m_playerBox );
	UpdateCameraPosition();

	m_playerPrevPosition = m_playerBox.m_centerPosition;
	m_keyboard.Update();
	m_loadingBar.SetPercentShown( m_numChunksInitialized * ONE_OVER_NUM_OF_INITIAL_CHUNKS );
	m_audioSystem->update();
}


//-----------------------------------------------------------------------------------------------
void World::RenderSkybox()
{
	m_glRenderer.PushMatrix();

	m_glRenderer.SetDepthMask( false );

	Color color( 1.f, 1.f, 1.f );
	if( m_timeOfDay < DAY_TRANSITION_START_TIME || m_timeOfDay > NIGHT_TRANSITION_END_TIME )
		color *= 0.1f;
	else if( m_timeOfDay < DAY_TRANSITION_END_TIME )
		color *= 1.f - ( ( DAY_TRANSITION_END_TIME - m_timeOfDay ) ) + 0.1f;
	else if( m_timeOfDay > NIGHT_TRANSITION_START_TIME )
		color *= 1.f - ( ( m_timeOfDay - NIGHT_TRANSITION_START_TIME ) ) + 0.1f;

	m_glRenderer.Translatef( m_camera.m_position.x - 0.5f, m_camera.m_position.y - 0.5f, m_camera.m_position.z - 0.5f );
	m_glRenderer.SetColor3f( color.r, color.g, color.b );
	m_glRenderer.EnableTexture2D();
	m_glRenderer.BindTexture2D( m_skyboxTexture->m_openglTextureID );

	m_glRenderer.BeginRender( QUADS );
	{
		// north face
		m_glRenderer.SetTexCoords2f( 0.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );

		m_glRenderer.SetTexCoords2f( ONE_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );

		m_glRenderer.SetTexCoords2f( ONE_OVER_SIX, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );

		m_glRenderer.SetTexCoords2f( 0.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );

		// east face
		m_glRenderer.SetTexCoords2f( ONE_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );

		m_glRenderer.SetTexCoords2f( TWO_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );

		m_glRenderer.SetTexCoords2f( TWO_OVER_SIX, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );

		m_glRenderer.SetTexCoords2f( ONE_OVER_SIX, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );

		// south face
		m_glRenderer.SetTexCoords2f( TWO_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );

		m_glRenderer.SetTexCoords2f( 0.5f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );

		m_glRenderer.SetTexCoords2f( 0.5f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );

		m_glRenderer.SetTexCoords2f( TWO_OVER_SIX, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );

		// west face
		m_glRenderer.SetTexCoords2f( 0.5f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );

		m_glRenderer.SetTexCoords2f( FOUR_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );

		m_glRenderer.SetTexCoords2f( FOUR_OVER_SIX, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );

		m_glRenderer.SetTexCoords2f( 0.5f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );

		// down face
		m_glRenderer.SetTexCoords2f( FOUR_OVER_SIX, 0.667f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );

		m_glRenderer.SetTexCoords2f( FIVE_OVER_SIX, 0.667f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );

		m_glRenderer.SetTexCoords2f( FIVE_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );

		m_glRenderer.SetTexCoords2f( FOUR_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );

		// up face
		m_glRenderer.SetTexCoords2f( FIVE_OVER_SIX, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );

		m_glRenderer.SetTexCoords2f( 1.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );

		m_glRenderer.SetTexCoords2f( 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );

		m_glRenderer.SetTexCoords2f( FIVE_OVER_SIX, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );
	}
	m_glRenderer.EndRender();

	m_glRenderer.SetDepthMask( true );
	m_glRenderer.DisableTexture2D();

	m_glRenderer.PopMatrix();
}


//-----------------------------------------------------------------------------------------------
void World::RenderSun()
{
	m_glRenderer.PushMatrix();

	m_glRenderer.SetDepthMask( false );
	m_glRenderer.Translatef( m_camera.m_position.x, m_camera.m_position.y + SUN_DISTANCE_FROM_CAMERA * sin( m_sunAngleRadians ) , m_camera.m_position.z + SUN_DISTANCE_FROM_CAMERA * cos( m_sunAngleRadians ) );
	m_glRenderer.Scalef( SUN_SIZE_SCALE, SUN_SIZE_SCALE, SUN_SIZE_SCALE );
	m_glRenderer.SetColor3f( 1.f, 1.f, 0.f );

	m_glRenderer.BeginRender( QUADS );
	{
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );

		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );

		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );

		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );

		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );

		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );
	}
	m_glRenderer.EndRender();

	m_glRenderer.SetDepthMask( true );

	m_glRenderer.PopMatrix();
}


//-----------------------------------------------------------------------------------------------
void World::RenderMoon()
{
	m_glRenderer.PushMatrix();

	m_glRenderer.SetDepthMask( false );
	m_glRenderer.Translatef( m_camera.m_position.x, m_camera.m_position.y  + MOON_DISTANCE_FROM_CAMERA * sin( m_moonAngleRadians ), m_camera.m_position.z + MOON_DISTANCE_FROM_CAMERA * cos( m_moonAngleRadians ) );
	m_glRenderer.Scalef( MOON_SIZE_SCALE, MOON_SIZE_SCALE, MOON_SIZE_SCALE );
	m_glRenderer.SetColor3f( 0.7f, 0.7f, 0.7f );

	m_glRenderer.BeginRender( QUADS );
	{
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );

		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );

		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );

		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );

		m_glRenderer.SetVertex3f( 1.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 0.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 0.f );
		m_glRenderer.SetVertex3f( 1.f, 0.f, 0.f );

		m_glRenderer.SetVertex3f( 1.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 0.f, 1.f );
		m_glRenderer.SetVertex3f( 0.f, 1.f, 1.f );
		m_glRenderer.SetVertex3f( 1.f, 1.f, 1.f );
	}
	m_glRenderer.EndRender();

	m_glRenderer.SetDepthMask( true );

	m_glRenderer.PopMatrix();
}


//-----------------------------------------------------------------------------------------------
void World::RenderChunks()
{
	Vector2 frustum;
	if( m_camera.m_orientation.pitch < 90.f && m_camera.m_orientation.pitch > -90.f )
	{
		frustum.x = cos( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) ) * cos( ConvertDegreesToRadians( m_camera.m_orientation.pitch ) );
		frustum.y = sin( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) ) * cos( ConvertDegreesToRadians( m_camera.m_orientation.pitch ) );
	}
	else
	{
		frustum.x = cos( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) );
		frustum.y = sin( ConvertDegreesToRadians( m_camera.m_orientation.yaw ) );
	}

	typedef std::map<IntVector2, Chunk*>::iterator mapIterator;
	for( mapIterator iter = m_chunks.begin(); iter != m_chunks.end(); ++iter )
	{
		Chunk* blockChunk = iter->second;

		if( blockChunk != nullptr )
		{
			Vector2 chunkPosition( (float) blockChunk->m_position.x, (float) blockChunk->m_position.y );
			if( !CheckForChunkCull( frustum, chunkPosition ) )
				blockChunk->Render();
		}
	}
}


//-----------------------------------------------------------------------------------------------
void World::RenderCrosshair()
{
	m_glRenderer.SetColor3f( 1.f, 1.f, 1.f );
	m_glRenderer.SetLineWidth( CROSSHAIR_LINE_WIDTH );
	m_glRenderer.DisableTexture2D();
	m_glRenderer.EnableBlend();
	m_glRenderer.BlendFunction( ONE_MINUS_DST_COLOR, ZERO );

	m_glRenderer.BeginRender( LINES );
	{
		m_glRenderer.SetVertex2f( -CROSSHAIR_LINE_LENGTH, 0.f );
		m_glRenderer.SetVertex2f( CROSSHAIR_LINE_LENGTH, 0.f );

		m_glRenderer.SetVertex2f( 0.f, -( CROSSHAIR_LINE_LENGTH * SIXTEEN_BY_NINE ) );
		m_glRenderer.SetVertex2f( 0.f, ( CROSSHAIR_LINE_LENGTH * SIXTEEN_BY_NINE ) );
	}
	m_glRenderer.EndRender();

	m_glRenderer.BlendFunction( SRC_ALPHA, ONE_MINUS_SRC_ALPHA );
}


//-----------------------------------------------------------------------------------------------
void World::RenderLoadScreen()
{
	m_loadingBar.Render();

	m_glRenderer.SetColor3f( 1.f, 1.f, 1.f );
	m_glRenderer.EnableTexture2D();
	m_glRenderer.BindTexture2D( m_loadScreenTexture->m_openglTextureID );

	m_glRenderer.BeginRender( QUADS );
	{
		m_glRenderer.SetTexCoords2f( 0.f, 1.f );
		m_glRenderer.SetVertex2f( -1.f, -1.f );

		m_glRenderer.SetTexCoords2f( 1.f, 1.f );
		m_glRenderer.SetVertex2f( 1.f, -1.f );

		m_glRenderer.SetTexCoords2f( 1.f, 0.f );
		m_glRenderer.SetVertex2f( 1.f, 1.f );

		m_glRenderer.SetTexCoords2f( 0.f, 0.f );
		m_glRenderer.SetVertex2f( -1.f, 1.f );
	}
	m_glRenderer.EndRender();
}

	
//-----------------------------------------------------------------------------------------------
void World::Render()
{
	if( !m_initialized )
	{
		RenderLoadScreen();
		return;
	}

	m_glRenderer.PushMatrix();

	m_glRenderer.ClearColorBufferBit();
	m_glRenderer.ClearDepthBufferBit();

	m_glRenderer.SetPerspective( FIELD_OF_VIEW_Y, SIXTEEN_BY_NINE, 0.1f, 1000.f );
	SetCameraPositionAndOrientation();

	RenderSkybox();
	RenderSun();
	RenderMoon();
	RenderChunks();

	if( m_traceResult.m_blockFirstHit != nullptr )
		m_traceResult.m_blockFirstHit->RenderOutline();

	m_glRenderer.PopMatrix();

	RenderCrosshair();
}