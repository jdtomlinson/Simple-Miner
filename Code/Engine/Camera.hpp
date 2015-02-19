#ifndef include_Camera
#define include_Camera
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Vector3.hpp"
#include "EulerAngles.hpp"


//-----------------------------------------------------------------------------------------------
class Camera
{
public:
	Camera();
	Camera( const Vector3& position );

	Vector3 m_position;
	EulerAngles m_orientation;
};


//-----------------------------------------------------------------------------------------------
inline Camera::Camera()
	: m_position( 0.f, 0.f, 0.f )
	, m_orientation( 0.f, 0.f, 0.f )
{

}


//-----------------------------------------------------------------------------------------------
inline Camera::Camera( const Vector3& position )
	: m_position( position )
	, m_orientation( 0.f, 0.f, 0.f )
{

}


#endif // include_Camera