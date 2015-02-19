#include "Mouse.hpp"
#include <windows.h>


//-----------------------------------------------------------------------------------------------
Mouse::Mouse()
{
	m_leftButtonDown = false;
	m_rightButtonDown = false;
}


//-----------------------------------------------------------------------------------------------
bool Mouse::IsLeftButtonClicked()
{
	if( ( GetKeyState( VK_LBUTTON ) & 0x8000 ) && !m_leftButtonDown )
	{
		m_leftButtonDown = true;
		return true;
	}
	else if( !( GetKeyState( VK_LBUTTON ) & 0x8000 ) )
	{
		m_leftButtonDown = false;
	}

	return false;
}


//-----------------------------------------------------------------------------------------------
bool Mouse::IsRightButtonClicked()
{
	if( ( GetKeyState( VK_RBUTTON ) & 0x8000 ) && !m_rightButtonDown )
	{
		m_rightButtonDown = true;
		return true;
	}
	else if( !( GetKeyState( VK_RBUTTON ) & 0x8000 ) )
	{
		m_rightButtonDown = false;
	}

	return false;
}


//-----------------------------------------------------------------------------------------------
Vector2 Mouse::GetCursorPosition()
{
	LPPOINT lpPoint = new POINT;
	Vector2 mousePosition;

	if( GetCursorPos( lpPoint ) )
	{
		mousePosition.x = (float)lpPoint->x;
		mousePosition.y = (float)lpPoint->y;
	}

	delete lpPoint;
	return mousePosition;
}


//-----------------------------------------------------------------------------------------------
void Mouse::SetCursorPosition( const Vector2& setPosition )
{
	SetCursorPos( (int) setPosition.x, (int) setPosition.y );
}