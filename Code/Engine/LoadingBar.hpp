#ifndef include_LoadingBar
#define include_LoadingBar
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Color.hpp"
#include "Vector2.hpp"
#include "OpenGLRenderer.hpp"


//-----------------------------------------------------------------------------------------------
class LoadingBar
{
public:
	LoadingBar();
	LoadingBar( const Vector2& bottomLeftPos, const Vector2& topRightPos, const Color& barColor );
	void SetPercentShown( float percent );
	void Render();

	Color			m_barColor;
	Vector2			m_bottomLeftCorner;
	Vector2			m_topRightCorner;
	float			m_percentShown;
	OpenGLRenderer	m_glRenderer;
};


#endif // include_LoadingBar