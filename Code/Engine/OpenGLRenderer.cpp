#include "OpenGLRenderer.hpp"


//-----------------------------------------------------------------------------------------------
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::Initialize()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress( "glGenBuffers" );
	glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress( "glBindBuffer" );
	glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress( "glBufferData" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress( "glDeleteBuffers" );
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress( "glGenerateMipmap" );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::PushMatrix()
{
	glPushMatrix();
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::PopMatrix()
{
	glPopMatrix();
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::EnableTexture2D()
{
	glEnable( GL_TEXTURE_2D );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::DisableTexture2D()
{
	glDisable( GL_TEXTURE_2D );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::GenerateTextures( unsigned int* textures )
{
	glGenTextures( 1, textures );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexture2DImage( int level, pixelDataFormat internalFormat, int width, int height, int border, pixelDataFormat bufferFormat, const void* pixels )
{
	GLenum interForm = RGB;
	GLenum buffForm = RGB;

	if( internalFormat == RGB )
		interForm = GL_RGB;
	else if( internalFormat == RGBA )
		interForm = GL_RGBA;

	if( bufferFormat == RGB )
		buffForm = GL_RGB;
	else if( bufferFormat == RGBA )
		buffForm = GL_RGBA;

	glTexImage2D( GL_TEXTURE_2D, level, interForm, width, height, border, buffForm, GL_UNSIGNED_BYTE, pixels );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::BindTexture2D( unsigned int texture )
{
	glBindTexture( GL_TEXTURE_2D, texture );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::GenerateMipmapTexture2D()
{
	glGenerateMipmap( GL_TEXTURE_2D );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::GenerateMipmapHint()
{
	glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::PixelStore()
{
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::Translatef( float transX, float transY, float transZ )
{
	glTranslatef( transX, transY, transZ );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::Rotatef( float angleDeg, float rotateX, float rotateY, float rotateZ )
{
	glRotatef( angleDeg, rotateX, rotateY, rotateZ );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::Scalef( float scaleX, float scaleY, float scaleZ )
{
	glScalef( scaleX, scaleY, scaleZ );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetColor3f( float red, float green, float blue )
{
	glColor3f( red, green, blue );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetColor4f( float red, float green, float blue, float alpha )
{
	glColor4f( red, green, blue, alpha );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetLineWidth( float lineWidth )
{
	glLineWidth( lineWidth );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetDepthMask( bool depthMaskOnOrOff )
{
	if( depthMaskOnOrOff )
		glDepthMask( GL_TRUE );
	else
		glDepthMask( GL_FALSE );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetPerspective( float fieldOfView, float aspect, float zNear, float zFar )
{
	gluPerspective( fieldOfView, aspect, zNear, zFar );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::BeginRender( renderType shape )
{
	if( shape == LINES )
		glBegin( GL_LINES );
	else if( shape == QUADS )
		glBegin( GL_QUADS );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::EndRender()
{
	glEnd();
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetVertex2f( float vertX, float vertY )
{
	glVertex2f( vertX, vertY );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetVertex3f( float vertX, float vertY, float vertZ )
{
	glVertex3f( vertX, vertY, vertZ );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexCoords2f( float coordX, float coordY )
{
	glTexCoord2f( coordX, coordY );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::GenerateBuffers( int size, unsigned int* buffers )
{
	glGenBuffers( size, buffers );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::DeleteBuffers( int size, unsigned int* buffers )
{
	glDeleteBuffers( size, buffers );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::BindBuffer( unsigned int buffer )
{
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetBufferData( int size, const void* data )
{
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::EnableClientState( arrayType aType )
{
	if( aType == VERTEX_ARRAY )
		glEnableClientState( GL_VERTEX_ARRAY );
	else if( aType == COLOR_ARRAY )
		glEnableClientState( GL_COLOR_ARRAY );
	else if( aType == TEXTURE_COORD_ARRAY )
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::DisableClientState( arrayType aType )
{
	if( aType == VERTEX_ARRAY )
		glDisableClientState( GL_VERTEX_ARRAY );
	else if( aType == COLOR_ARRAY )
		glDisableClientState( GL_COLOR_ARRAY );
	else if( aType == TEXTURE_COORD_ARRAY )
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetVertexPointer( int size, unsigned int stride, const void* pointer )
{
	glVertexPointer( size, GL_FLOAT, stride, pointer );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetColorPointer( int size, unsigned int stride, const void* pointer )
{
	glColorPointer( size, GL_FLOAT, stride, pointer );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexCoordPointer( int size, unsigned int stride, const void* pointer )
{
	glTexCoordPointer( size, GL_FLOAT, stride, pointer );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::DrawArrays( renderType shape, int first, unsigned int count )
{
	if( shape == QUADS )
		glDrawArrays( GL_QUADS, first, count );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::EnableCullFace()
{
	glEnable( GL_CULL_FACE );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::CullFrontFaceDirection( frontFaceDirection dirForFrontFace )
{
	if( dirForFrontFace == CLOCKWISE )
		glFrontFace( GL_CW );
	else if( dirForFrontFace == COUNTER_CLOCKWISE )
		glFrontFace( GL_CCW );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::CullFaceSide( faceSide frontOrBack )
{
	if( frontOrBack == FRONT )
		glCullFace( GL_FRONT );
	else if( frontOrBack == BACK )
		glCullFace( GL_BACK );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::ClearColorBufferBit()
{
	glClear( GL_COLOR_BUFFER_BIT );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::ClearDepthBufferBit()
{
	glClear( GL_DEPTH_BUFFER_BIT );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetClearColor( float red, float green, float blue, float alpha )
{
	glClearColor( red, green, blue, alpha );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetClearDepth( float depth )
{
	glClearDepth( depth );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::EnableBlend()
{
	glEnable( GL_BLEND );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::EnableDepthTest()
{
	glEnable( GL_DEPTH_TEST );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::BlendFunction( blendFuncParam sFactor, blendFuncParam dFactor )
{
	int sFact = GetBlendParameter( sFactor );
	int dFact = GetBlendParameter( dFactor );

	glBlendFunc( sFact, dFact );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexture2DWrapS( textureWrap clampOrRepeat )
{
	if( clampOrRepeat == CLAMP )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	else if( clampOrRepeat == REPEAT )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexture2DWrapT( textureWrap clampOrRepeat )
{
	if( clampOrRepeat == CLAMP )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	else if( clampOrRepeat == REPEAT )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexture2DMagnificationFilter( textureFilter filter )
{
	if( filter == NEAREST )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	else if( filter == LINEAR )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	else if( filter == NEAREST_MIPMAP_NEAREST )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	else if( filter == NEAREST_MIPMAP_LINEAR )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR );
	else if( filter == LINEAR_MIPMAP_NEAREST )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	else if( filter == LINEAR_MIPMAP_LINEAR )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexture2DMinificationFilter( textureFilter filter )
{
	if( filter == NEAREST )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	else if( filter == LINEAR )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	else if( filter == NEAREST_MIPMAP_NEAREST )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
	else if( filter == NEAREST_MIPMAP_LINEAR )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
	else if( filter == LINEAR_MIPMAP_NEAREST )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	else if( filter == LINEAR_MIPMAP_LINEAR )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
}


//-----------------------------------------------------------------------------------------------
void OpenGLRenderer::SetTexture2DMaxLevel( int param )
{
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, param );
}


//-----------------------------------------------------------------------------------------------
int OpenGLRenderer::GetBlendParameter( blendFuncParam param )
{
	if( param == ZERO )
		return GL_ZERO;
	if( param == ONE )
		return GL_ONE;
	if( param == DST_COLOR )
		return GL_DST_COLOR;
	if( param == ONE_MINUS_DST_COLOR )
		return GL_ONE_MINUS_DST_COLOR;
	if( param == SRC_ALPHA )
		return GL_SRC_ALPHA;
	if( param == ONE_MINUS_SRC_ALPHA )
		return GL_ONE_MINUS_SRC_ALPHA;

	return GL_ZERO;
}