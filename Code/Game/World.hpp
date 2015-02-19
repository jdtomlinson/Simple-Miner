#ifndef include_World
#define include_World
#pragma once

//-----------------------------------------------------------------------------------------------
#include <set>
#include "Block.hpp"
#include "Chunk.hpp"
#include "TraceResult.hpp"
#include "GameCommon.hpp"
#include "../Engine/AABB3.hpp"
#include "../Engine/Mouse.hpp"
#include "../Engine/Camera.hpp"
#include "../Engine/Vector2.hpp"
#include "../Engine/Texture.hpp"
#include "../Engine/Keyboard.hpp"
#include "../Engine/LoadingBar.hpp"
#include "../Engine/OpenGLRenderer.hpp"
#include "../Engine/fmod.hpp"
#pragma comment( lib, "fmodex_vc.lib" )


//-----------------------------------------------------------------------------------------------
const int XBOX_CONTROLLER_PLAYER_ONE_NUMBER = 0;
const int NUM_CHUNKS_X_AXIS = 1;
const int NUM_CHUNKS_Y_AXIS = 1;
const int VIEW_RADIUS_LENGTH_IN_CHUNKS = 16;
const int REMOVE_RADIUS_LENGTH_IN_CHUNKS = 20;
const int NUM_CHUNKS_TO_ADD_PER_FRAME = 3;
const int NUM_CHUNKS_TO_REMOVE_PER_FRAME = 3;
const int NUM_CHUNKS_TO_UPDATE_PER_FRAME = 3;
const float ONE_OVER_NUM_CHUNKS_Y_AXIS = 1.f / (float) NUM_CHUNKS_Y_AXIS;
const float ONE_OVER_NUM_OF_INITIAL_CHUNKS = 1.f / ( VIEW_RADIUS_LENGTH_IN_CHUNKS * VIEW_RADIUS_LENGTH_IN_CHUNKS * pi );
const float WALKING_MOVE_SPEED_BLOCKS_PER_SECOND = 5.f;
const float WALKING_MOVE_SPEED_OFF_GROUND_BLOCKS_PER_SECOND = 3.f;
const float FLYING_MOVE_SPEED_BLOCKS_PER_SECOND = 10.f;
const float NO_CLIP_MOVE_SPEED_BLOCKS_PER_SECOND = 10.f;
const float TIME_TO_FULL_SPEED_FULL_STOP = 0.15f;
const float ROTATION_DEGREES_PER_SECOND = 70.f;
const float GRAVITY_ACCELERATION_BLOCKS_PER_SECOND = 4.f;
const float JUMP_SPEED_BLOCKS_PER_SECOND = 1.75f;
const float BOUNDING_BOX_X_AXIS_LENGTH = 0.6f;
const float BOUNDING_BOX_Y_AXIS_LENGTH = 0.6f;
const float BOUNDING_BOX_Z_AXIS_LENGTH = 1.85f;
const float CAMERA_DISTANCE_FROM_BOX_BOTTOM = 1.62f;
const float FIELD_OF_VIEW_Y = 45.f;
const float RAY_CAST_LENGTH_IN_BLOCKS = 8.f;
const float SECONDS_BETWEEN_SONGS = 60.f;
const float SUN_MOON_FULL_ROTATION_MINUTES = 10.f;
const float CROSSHAIR_LINE_WIDTH = 8.f;
const float CROSSHAIR_LINE_LENGTH = 0.04f;
const float SUN_MOON_FULL_ROTATION_SECONDS = SUN_MOON_FULL_ROTATION_MINUTES * 60.f;
const float TWENTYFOUR_OVER_ROTATION_SECONDS = 24.f / SUN_MOON_FULL_ROTATION_SECONDS;
const float SUN_DISTANCE_FROM_CAMERA = 20.f;
const float MOON_DISTANCE_FROM_CAMERA = 20.f;
const float SUN_SIZE_SCALE = 1.f;
const float MOON_SIZE_SCALE = 1.f;
const float TIME_OF_DAY_AT_START = 12.f;
const float DAY_TRANSITION_START_TIME = 5.5f;
const float DAY_TRANSITION_END_TIME = 6.5f;
const float DAY_TRANSITION_MIDDLE_TIME = ( ( DAY_TRANSITION_END_TIME - DAY_TRANSITION_START_TIME ) * 0.65f ) + DAY_TRANSITION_START_TIME;
const float NIGHT_TRANSITION_START_TIME = 17.5f;
const float NIGHT_TRANSITION_END_TIME = 18.5f;
const float NIGHT_TRANSITION_MIDDLE_TIME = ( ( NIGHT_TRANSITION_END_TIME - NIGHT_TRANSITION_START_TIME ) * 0.65f ) + NIGHT_TRANSITION_START_TIME;
const unsigned char NIGHT_SKYBOX_LIGHT_VALUE = 4;
const Color LOADING_BAR_COLOR = Color( 0.f, 1.f, 0.f );
const Vector2 LOADING_BAR_BOTTOM_LEFT_POSITION = Vector2( -0.316f, -0.09f );
const Vector2 LOADING_BAR_TOP_RIGHT_POSITION = Vector2( 0.306f, -0.185f );
const Vector3 BOUNDING_BOX_START_POSITION = Vector3( 0.f, 0.f, 128.f );


//-----------------------------------------------------------------------------------------------
enum movementMode {
	WALKING,
	FLYING,
	NO_CLIP,
};


//-----------------------------------------------------------------------------------------------
class World
{
public:
	World( float worldWith, float worldHeight );
	FMOD::Sound* LoadAudio( char* fileName );
	void InitializeAudio();
	void Initialize();
	void Deconstruct();
	bool ProcessKeyDownEvent( unsigned char virtualKeyCode );
	bool ProcessKeyUpEvent( unsigned char virtualKeyCode );
	Block* GetBlock( const Vector3& blockPosition );
	Chunk* GetChunkBlockIsIn( Block* block );
	TraceResult CastRay( const Vector3& start, const Vector3& end );
	void SetCameraPositionAndOrientation();
	void SetSurroundingChunks( Chunk* blockChunk );
	void SetLightForAddedChunk( Chunk* blockChunk );
	void SetLightForEnclosedAirBlocks( Block* block );
	void SetLightForRemovedBlock( Block* block );
	void SetLightForAddedBlock( Block* block );
	void RemoveBlock( Block* block );
	void AddBlock( Block* block, unsigned char blockType );
	void CheckForBlockRemoval();
	void CheckForBlockAdd();
	void RemoveChunk( Chunk* blockChunk );
	void AddChunk( const IntVector2& chunkPosition );
	void CheckForChunkRemoval();
	void CheckForChunkAdd();
	void SetBlocksBelowToSeeSky( Block* block );
	void SetBlocksBelowToNotSeeSky( Block* block );
	void SetFaceHighlighting();
	void CheckForSkyboxLightChange();
	std::set<Block*> GetBlocksBoundingBoxIsIn( const AABB3& bouningBox );
	bool CheckForChunkCull( const Vector2& frustum, const Vector2& chunkPosition );
	bool CheckIfBoundingBoxIsOnlyInAir( const AABB3& boundingBox );
	bool CheckIfBoundingBoxIsInBlock( const AABB3& boundingBox, Block* block );
	bool CheckIfBoundingBoxIsOnGround( const AABB3& boundingBox );
	bool CheckForNorthFaceCollision( const AABB3& boundingBox );
	bool CheckForSouthFaceCollision( const AABB3& boundingBox );
	bool CheckForWestFaceCollision( const AABB3& boundingBox );
	bool CheckForEastFaceCollision( const AABB3& boundingBox );
	bool CheckForTopFaceCollision( const AABB3& boundingBox );
	bool CheckForBottomFaceCollision( const AABB3& boundingBox );
	void AdjustBoundingBoxForNorthSouthCollision( AABB3& boundingBox );
	void AdjustBoundingBoxForEastWestCollision( AABB3& boundingBox );
	void AdjustBoundingBoxForUpDownCollision( AABB3& boundingBox );
	void AdjustBoundingBoxForCollision( AABB3& boundingBox );
	void PlayWalkSound();
	void PlayBlockDigSound( Block* block );
	void PlayBlockPlaceSound();
	void UpdateBackgroundMusic( float deltaSeconds );
	void UpdateSkyboxLightValue();
	void UpdateSurroundingChunks( Chunk* blockChunk );
	void UpdateChunks();
	void UpdateFromInput( float deltaSeconds );
	void UpdateCameraPosition();
	void UpdateRayCasting();
	void UpdateChunkBuffers( Chunk* blockChunk );
	void UpdateTimeOfDay( float deltaSeconds );
	void Update();
	void RenderSkybox();
	void RenderSun();
	void RenderMoon();
	void RenderChunks();
	void RenderCrosshair();
	void RenderLoadScreen();
	void Render();

	Camera							m_camera;
	Mouse							m_mouse;
	Keyboard						m_keyboard;
	LoadingBar						m_loadingBar;
	AABB3							m_playerBox;
	Vector3							m_playerPrevPosition;
	movementMode					m_movementMode;
	bool							m_isOnGround;
	bool							m_isMoving;
	bool							m_initialized;
	bool							m_changedToDay;
	bool							m_changedToNight;
	std::map<IntVector2, Chunk*>	m_chunks;
	std::map<IntVector2, float>		m_chunkSquaredDistances;
	std::vector<IntVector2>			m_chunksToAdd;
	std::vector<IntVector2>			m_chunksToRemove;
	TraceResult						m_traceResult;
	Texture*						m_skyboxTexture;
	Texture*						m_loadScreenTexture;
	float							m_numChunksInitialized;
	float							m_fallSpeed;
	float							m_secondsSinceMoveStart;
	float							m_secondsSinceMoveStop;
	float							m_timeOfDay;
	float							m_secondsSinceSongEnd;
	float							m_sunAngleRadians;
	float							m_moonAngleRadians;
	Vector2							m_size;
	Vector3							m_prevCameraForwardXY;
	Vector3							m_prevcameraLeftXY;
	OpenGLRenderer					m_glRenderer;
	FMOD::System*					m_audioSystem;
	FMOD::Channel*					m_musicChannel;
	FMOD::Channel*					m_walkChannel;
	FMOD::Channel*					m_digChannel;
	std::vector<FMOD::Sound*>		m_musicSounds;
	std::vector<FMOD::Sound*>		m_walkDirtSounds;
	std::vector<FMOD::Sound*>		m_walkStoneSounds;
	std::vector<FMOD::Sound*>		m_digDirtSounds;
	std::vector<FMOD::Sound*>		m_digStoneSounds;
	FMOD::Sound*					m_placeSound;
	FMOD::Sound*					m_stayNoClipSound;
};


#endif // include_World