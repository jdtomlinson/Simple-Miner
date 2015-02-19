#ifndef include_Renderer
#define include_Renderer
#pragma once

//-----------------------------------------------------------------------------------------------
enum renderType {
	LINES,
	QUADS,
};


//-----------------------------------------------------------------------------------------------
enum arrayType {
	VERTEX_ARRAY,
	COLOR_ARRAY,
	TEXTURE_COORD_ARRAY,
};


//-----------------------------------------------------------------------------------------------
enum frontFaceDirection {
	CLOCKWISE,
	COUNTER_CLOCKWISE,
};


//-----------------------------------------------------------------------------------------------
enum faceSide {
	FRONT,
	BACK,
};


//-----------------------------------------------------------------------------------------------
enum textureWrap {
	CLAMP,
	REPEAT,
};


//-----------------------------------------------------------------------------------------------
enum textureFilter {
	NEAREST,
	LINEAR,
	NEAREST_MIPMAP_NEAREST,
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_NEAREST,
	LINEAR_MIPMAP_LINEAR,
};


//-----------------------------------------------------------------------------------------------
enum pixelDataFormat {
	RGB,
	RGBA,
};


//-----------------------------------------------------------------------------------------------
enum blendFuncParam {
	ZERO,
	ONE,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
};


//-----------------------------------------------------------------------------------------------
class Renderer
{
public:
	Renderer() {}
	virtual void Initialize() {}
	virtual void PushMatrix() {}
	virtual void PopMatrix() {}
	virtual void EnableTexture2D() {}
	virtual void DisableTexture2D() {}
	virtual void GenerateTextures() {}
	virtual void SetTexture2DImage() {}
	virtual void BindTexture2D() {}
	virtual void GenerateMipmapTexture2D() {}
	virtual void GenerateMipmapHint() {}
	virtual void PixelStore() {}
	virtual void Translatef() {}
	virtual void Rotatef() {}
	virtual void Scalef() {}
	virtual void SetColor3f() {}
	virtual void SetColor4f() {}
	virtual void SetLineWidth() {}
	virtual void SetDepthMask() {}
	virtual void SetPerspective() {}
	virtual void BeginRender() {}
	virtual void EndRender() {}
	virtual void SetVertex2f() {}
	virtual void SetVertex3f() {}
	virtual void SetTexCoords2f() {}
	virtual void GenerateBuffers() {}
	virtual void DeleteBuffers() {}
	virtual void BindBuffer() {}
	virtual void SetBufferData() {}
	virtual void EnableClientState() {}
	virtual void DisableClientState() {}
	virtual void SetVertexPointer() {}
	virtual void SetColorPointer() {}
	virtual void SetTexCoordPointer() {}
	virtual void DrawArrays() {}
	virtual void EnableCullFace() {}
	virtual void CullFrontFaceDirection() {}
	virtual void CullFaceSide() {}
	virtual void ClearColorBufferBit() {}
	virtual void ClearDepthBufferBit() {}
	virtual void SetClearColor() {}
	virtual void SetClearDepth() {}
	virtual void EnableBlend() {}
	virtual void EnableDepthTest() {}
	virtual void BlendFunction() {}
	virtual void SetTexture2DWrapS() {}
	virtual void SetTexture2DWrapT() {}
	virtual void SetTexture2DMagnificationFilter() {}
	virtual void SetTexture2DMinificationFilter() {}
	virtual void SetTexture2DMaxLevel() {}
};


#endif // include_Renderer