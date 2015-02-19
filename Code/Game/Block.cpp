#include "Block.hpp"


//-----------------------------------------------------------------------------------------------
Block::Block()
	: m_blockType( AIR )
	, m_position( 0.f, 0.f, 0.f )
	, m_internalLightValue( 0 )
{
	
}


//-----------------------------------------------------------------------------------------------
Block::Block( unsigned char blockType, Vector3 centerPosition )
	: m_blockType( blockType )
	, m_position( centerPosition.x, centerPosition.y, centerPosition.z )
	, m_internalLightValue( 0 )
{

}


//-----------------------------------------------------------------------------------------------
void Block::RenderOutline()
{
	float posMinX = m_position.x - 0.001f;
	float posMaxX = m_position.x + 1.001f;
	float posMinY = m_position.y - 0.001f;
	float posMaxY = m_position.y + 1.001f;
	float posMinZ = m_position.z - 0.001f;
	float posMaxZ = m_position.z + 1.001f;

	m_glRenderer.SetLineWidth( 5.f );
	m_glRenderer.SetColor3f( 0.f, 0.f, 0.f );

	m_glRenderer.BeginRender( LINES );
	{
		m_glRenderer.SetVertex3f( posMinX, posMinY, posMinZ );
		m_glRenderer.SetVertex3f( posMaxX, posMinY, posMinZ );

		m_glRenderer.SetVertex3f( posMinX, posMinY, posMinZ );
		m_glRenderer.SetVertex3f( posMinX, posMaxY, posMinZ );

		m_glRenderer.SetVertex3f( posMinX, posMinY, posMinZ );
		m_glRenderer.SetVertex3f( posMinX, posMinY, posMaxZ );

		m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMinZ );
		m_glRenderer.SetVertex3f( posMinX, posMaxY, posMinZ );

		m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMinZ );
		m_glRenderer.SetVertex3f( posMaxX, posMinY, posMinZ );

		m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMinZ );
		m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMaxZ );

		m_glRenderer.SetVertex3f( posMaxX, posMinY, posMinZ );
		m_glRenderer.SetVertex3f( posMaxX, posMinY, posMaxZ );

		m_glRenderer.SetVertex3f( posMinX, posMaxY, posMinZ );
		m_glRenderer.SetVertex3f( posMinX, posMaxY, posMaxZ );

		m_glRenderer.SetVertex3f( posMinX, posMinY, posMaxZ );
		m_glRenderer.SetVertex3f( posMaxX, posMinY, posMaxZ );

		m_glRenderer.SetVertex3f( posMinX, posMinY, posMaxZ );
		m_glRenderer.SetVertex3f( posMinX, posMaxY, posMaxZ );

		m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMaxZ );
		m_glRenderer.SetVertex3f( posMinX, posMaxY, posMaxZ );

		m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMaxZ );
		m_glRenderer.SetVertex3f( posMaxX, posMinY, posMaxZ );
	}
	m_glRenderer.EndRender();

	m_glRenderer.BeginRender( QUADS );
	{
		m_glRenderer.SetColor4f( 0.8f, 0.8f, 0.2f, 0.3f );
		if( m_highlightFaceFlag & CULL_NEGATIVE_X_FACE )
		{
			m_glRenderer.SetVertex3f( posMinX, posMinY, posMaxZ );
			m_glRenderer.SetVertex3f( posMinX, posMaxY, posMaxZ );
			m_glRenderer.SetVertex3f( posMinX, posMaxY, posMinZ );
			m_glRenderer.SetVertex3f( posMinX, posMinY, posMinZ );
		}
		else if( m_highlightFaceFlag & CULL_POSITIVE_X_FACE )
		{
			m_glRenderer.SetVertex3f( posMaxX, posMinY, posMinZ );
			m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMinZ );
			m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMaxZ );
			m_glRenderer.SetVertex3f( posMaxX, posMinY, posMaxZ );
		}
		else if( m_highlightFaceFlag & CULL_NEGATIVE_Y_FACE )
		{
			m_glRenderer.SetVertex3f( posMinX, posMinY, posMaxZ );
			m_glRenderer.SetVertex3f( posMinX, posMinY, posMinZ );
			m_glRenderer.SetVertex3f( posMaxX, posMinY, posMinZ );
			m_glRenderer.SetVertex3f( posMaxX, posMinY, posMaxZ );
		}
		else if( m_highlightFaceFlag & CULL_POSITIVE_Y_FACE )
		{
			m_glRenderer.SetVertex3f( posMinX, posMaxY, posMinZ );
			m_glRenderer.SetVertex3f( posMinX, posMaxY, posMaxZ );
			m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMaxZ );
			m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMinZ );
		}
		else if( m_highlightFaceFlag & CULL_NEGATIVE_Z_FACE )
		{
			m_glRenderer.SetVertex3f( posMinX, posMinY, posMinZ );
			m_glRenderer.SetVertex3f( posMinX, posMaxY, posMinZ );
			m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMinZ );
			m_glRenderer.SetVertex3f( posMaxX, posMinY, posMinZ );
		}
		else if( m_highlightFaceFlag & CULL_POSITIVE_Z_FACE )
		{
			m_glRenderer.SetVertex3f( posMinX, posMaxY, posMaxZ );
			m_glRenderer.SetVertex3f( posMinX, posMinY, posMaxZ );
			m_glRenderer.SetVertex3f( posMaxX, posMinY, posMaxZ );
			m_glRenderer.SetVertex3f( posMaxX, posMaxY, posMaxZ );
		}
	}
	m_glRenderer.EndRender();
}