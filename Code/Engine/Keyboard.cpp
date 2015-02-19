#include "Keyboard.hpp"


//-----------------------------------------------------------------------------------------------
Keyboard::Keyboard()
{
	for( int key = 0; key < NUM_VIRTUAL_KEYS; ++key )
	{
		m_isKeyDown[ key ] = false;
		m_wasKeyDown[ key ] = false;
	}
}


//-----------------------------------------------------------------------------------------------
void Keyboard::SetKeyUp( unsigned char checkKeys )
{
	m_isKeyDown[ checkKeys ] = false;
}


//-----------------------------------------------------------------------------------------------
void Keyboard::SetKeyDown( unsigned char checkKeys )
{
	m_isKeyDown[ checkKeys ] = true;
}



//-----------------------------------------------------------------------------------------------
bool Keyboard::IsKeyPressedDown( keys checkKeys )
{
	if( m_isKeyDown[ checkKeys ] )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
bool Keyboard::WasKeyPressedDown( keys checkKeys )
{
	if( m_wasKeyDown[ checkKeys ] )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
void Keyboard::Update()
{
	memcpy( &m_wasKeyDown, &m_isKeyDown, sizeof( m_isKeyDown ) );
}