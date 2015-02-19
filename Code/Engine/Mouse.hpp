#ifndef include_Mouse
#define include_Mouse
#pragma once

//-----------------------------------------------------------------------------------------------
#include "Vector2.hpp"


//-----------------------------------------------------------------------------------------------
class Mouse
{
public:
	Mouse();
	bool IsLeftButtonClicked();
	bool IsRightButtonClicked();
	Vector2 GetCursorPosition();
	void SetCursorPosition( const Vector2& setPosition );

	bool		m_leftButtonDown;
	bool		m_rightButtonDown;
};


#endif // include_Mouse