#include "LoadingBar.hpp"


//-----------------------------------------------------------------------------------------------
LoadingBar::LoadingBar()
	: m_barColor()
	, m_bottomLeftCorner()
	, m_topRightCorner()
	, m_percentShown( 0.f )
{

}


//-----------------------------------------------------------------------------------------------
LoadingBar::LoadingBar( const Vector2& bottomLeftPos, const Vector2& topRightPos, const Color& barColor )
	: m_barColor( barColor )
	, m_bottomLeftCorner( bottomLeftPos )
	, m_topRightCorner( topRightPos )
	, m_percentShown( 0.f )
{

}


//-----------------------------------------------------------------------------------------------
void LoadingBar::SetPercentShown( float percent )
{
	m_percentShown = percent;
	if( m_percentShown > 1.f )
		m_percentShown = 1.f;
	if( m_percentShown < 0.f )
		m_percentShown = 0.f;
}


//-----------------------------------------------------------------------------------------------
void LoadingBar::Render()
{
	m_glRenderer.SetColor3f( m_barColor.r, m_barColor.g, m_barColor.b );
	m_glRenderer.DisableTexture2D();

	float rightPos = ( m_topRightCorner.x - m_bottomLeftCorner.x ) * m_percentShown;

	m_glRenderer.BeginRender( QUADS );
	{
		m_glRenderer.SetVertex2f( m_bottomLeftCorner.x, m_bottomLeftCorner.y );
		m_glRenderer.SetVertex2f( m_bottomLeftCorner.x, m_topRightCorner.y );
		m_glRenderer.SetVertex2f( m_bottomLeftCorner.x + rightPos, m_topRightCorner.y );
		m_glRenderer.SetVertex2f( m_bottomLeftCorner.x + rightPos, m_bottomLeftCorner.y );
	}
	m_glRenderer.EndRender();
}