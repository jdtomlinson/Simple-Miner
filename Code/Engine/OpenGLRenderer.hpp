#ifndef include_OpenGLRenderer
#define include_OpenGLRenderer
#pragma once

//-----------------------------------------------------------------------------------------------
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "Renderer.hpp"
#include "glext.h"


//-----------------------------------------------------------------------------------------------
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;


//-----------------------------------------------------------------------------------------------
class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer() {}
	void Initialize();
	void PushMatrix();
	void PopMatrix();
	void EnableTexture2D();
	void DisableTexture2D();
	void GenerateTextures( unsigned int* textures );
	void SetTexture2DImage( int level, pixelDataFormat internalFormat, int width, int height, int border, pixelDataFormat bufferFormat, const void* pixels );
	void BindTexture2D( unsigned int texture );
	void GenerateMipmapTexture2D();
	void GenerateMipmapHint();
	void PixelStore();
	void Translatef( float transX, float transY, float transZ );
	void Rotatef( float angleDeg, float rotateX, float rotateY, float rotateZ );
	void Scalef( float scaleX, float scaleY, float scaleZ );
	void SetColor3f( float red, float green, float blue );
	void SetColor4f( float red, float green, float blue, float alpha );
	void SetLineWidth( float lineWidth );
	void SetDepthMask( bool depthMaskOnOrOff );
	void SetPerspective( float fieldOfView, float aspect, float zNear, float zFar );
	void BeginRender( renderType shape );
	void EndRender();
	void SetVertex2f( float vertX, float vertY );
	void SetVertex3f( float vertX, float vertY, float vertZ );
	void SetTexCoords2f( float coordX, float coordY );
	void GenerateBuffers( int size, unsigned int* buffers );
	void DeleteBuffers( int size, unsigned int* buffers );
	void BindBuffer( unsigned int buffer );
	void SetBufferData( int size, const void* data );
	void EnableClientState( arrayType aType );
	void DisableClientState( arrayType aType );
	void SetVertexPointer( int size, unsigned int stride, const void* pointer );
	void SetColorPointer( int size, unsigned int stride, const void* pointer );
	void SetTexCoordPointer( int size, unsigned int stride, const void* pointer );
	void DrawArrays( renderType shape, int first, unsigned int count );
	void EnableCullFace();
	void CullFrontFaceDirection( frontFaceDirection dirForFontFace );
	void CullFaceSide( faceSide frontOrBack );
	void ClearColorBufferBit();
	void ClearDepthBufferBit();
	void SetClearColor( float red, float green, float blue, float alpha );
	void SetClearDepth( float depth );
	void EnableBlend();
	void EnableDepthTest();
	void BlendFunction(  blendFuncParam sFactor, blendFuncParam dFactor  );
	void SetTexture2DWrapS( textureWrap clampOrRepeat );
	void SetTexture2DWrapT( textureWrap clampOrRepeat );
	void SetTexture2DMagnificationFilter( textureFilter filter );
	void SetTexture2DMinificationFilter( textureFilter filter );
	void SetTexture2DMaxLevel( int param );

private:
	int GetBlendParameter( blendFuncParam param );
};


#endif // include_OpenGLRenderer