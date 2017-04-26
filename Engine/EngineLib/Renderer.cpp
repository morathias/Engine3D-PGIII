#include "Renderer.h"
#include "pg2_vertexbuffer.h"
#include "pg2_indexbuffer.h"
#include <iostream>
//==================================================================================
#include <d3d9.h>
#pragma comment (lib, "d3d9.lib") 
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib") 

#define _USE_MATH_DEFINES
#include <math.h>
//==================================================================================
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)
#define TEXTUREFVF (D3DFVF_XYZ | D3DFVF_TEX1)
//---------------------------primitive assignment-------------------------------
D3DPRIMITIVETYPE _primitives[PRIMITIVE_COUNT] =
{
	{D3DPT_POINTLIST},
	{D3DPT_LINELIST},
	{D3DPT_LINESTRIP},
	{D3DPT_TRIANGLELIST},
	{D3DPT_TRIANGLESTRIP},
	{D3DPT_TRIANGLEFAN,}
};
//---------------------------Matrix Type assignment-------------------------------
D3DTRANSFORMSTATETYPE _matrixType[MATRIX_TYPE_COUNT] =
{
	{ D3DTS_VIEW },
	{ D3DTS_PROJECTION },
	{ D3DTS_WORLD }
};
//==================================================================================
Renderer::Renderer(){}
//==================================================================================
Renderer::~Renderer(){
	m_pkDevice->Release();
	m_pkDevice = NULL;

	m_pkD3D->Release();
	m_pkD3D = NULL;

	delete v_buffer;
	v_buffer = NULL;
	delete texturedVBuffer;

	std::vector<Texture>::iterator iter;
	for(iter = _textureList.begin(); iter != _textureList.end(); iter++){
		(*iter)->Release();
		(*iter) = NULL;
	}

	_textureList.clear();
}
//==================================================================================
bool Renderer::init(HWND hWnd, unsigned int uiW, unsigned int uiH){
	_projectionMatrix = new D3DXMATRIX();
	uiWidth = uiW;
	uiHeight = uiH;
	_hwnd = hWnd;
	//------------------directX cretion-------------------------------------
	m_pkD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pkD3D) return false;
	//-----------------display creation-------------------------------------
	D3DDISPLAYMODE displayMode;
	HRESULT hr;
	hr = m_pkD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

	if (hr != D3D_OK) return false;
	//------------------directX present struct------------------------------
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//------------------device creation-------------------------------------
	hr = m_pkD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pkDevice);

	if (hr != D3D_OK) return false;
	//----------------------States-----------------------------------------
	m_pkDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pkDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pkDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pkDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pkDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pkDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pkDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//-----------------viewport creation-----------------------------------
	D3DVIEWPORT9 viewport;
	m_pkDevice->GetViewport(&viewport);

	float viewportWidth = static_cast<float>(viewport.Width);
	float viewportHeight = static_cast<float>(viewport.Height);
	//----------------orthogonal/perspective projection--------------------------------
	//D3DXMatrixOrthoLH(&projectionMatrix, viewportWidth, viewportHeight, 0.0f, 1000.0f);
	D3DXMatrixPerspectiveFovLH(_projectionMatrix, D3DX_PI * 0.25f, (FLOAT)viewportWidth / viewportHeight, (FLOAT)0.1f, (FLOAT)2000.0f);
	m_pkDevice->SetTransform(D3DTS_PROJECTION, _projectionMatrix);
	//----------------vertex Buffers---------------------------------------
	v_buffer = new pg1::VertexBuffer(m_pkDevice, sizeof(Vertex), CUSTOMFVF);
	texturedVBuffer = new pg1::VertexBuffer(m_pkDevice, sizeof(TexturedVertex), TEXTUREFVF);

	return true;
}
//==================================================================================
void Renderer::beginFrame(){
	m_pkDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(50, 50, 50), 1.0f, 0);
	m_pkDevice->BeginScene();
}
//==================================================================================
void Renderer::draw(Vertex* gameVertex, Primitive primitive, int vertexCount){
	v_buffer->bind();
	v_buffer->draw(gameVertex, _primitives[primitive], vertexCount);
}
//==================================================================================
void Renderer::draw(TexturedVertex* gameVertex, Primitive primitive, int vertexCount){
	texturedVBuffer->bind();
	texturedVBuffer->draw(gameVertex, _primitives[primitive], vertexCount);
}
//==================================================================================
Font& Renderer::createFont(int charSize, std::string textFont, bool italic){
	Font* font = new Font();
	D3DXCreateFont(m_pkDevice, 
				   charSize, 
				   0, 
				   FW_NORMAL, 
				   1, italic, 
				   DEFAULT_CHARSET, 
				   OUT_DEFAULT_PRECIS, 
				   ANTIALIASED_QUALITY, 
				   FF_DONTCARE,
				   textFont.c_str(),
				   font);

	return *font;
}
//==================================================================================
RECT& Renderer::createRect(int x, int y, int width, int height){
	RECT* rect = new RECT();
	SetRect(rect, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
	return *rect;
}
//==================================================================================
void Renderer::displayText(Font& font, RECT& rect, std::string text){
	font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255,255,255));
}
//==================================================================================
void Renderer::drawLine(float origin[2], float end[2], float thickness){
	DX9Line line;
	D3DXCreateLine(m_pkDevice, &line);

	D3DXVECTOR2 vertices[] = { D3DXVECTOR2(origin[0], origin[1]),
							   D3DXVECTOR2(end[0], end[1]) };

	line->SetWidth(thickness);
	line->Begin();
	line->Draw(vertices, ARRAYSIZE(vertices), D3DCOLOR_ARGB(255, 255, 255, 255));
	line->End();
	line->Release();
}
//==================================================================================
void Renderer::drawRect(float originX, float originY, float width, float height, float thickness){
	DX9Line line;
	D3DXCreateLine(m_pkDevice, &line);

	D3DXVECTOR2 vertices[] = {D3DXVECTOR2(originX, originY),
							  D3DXVECTOR2(width + originX, originY),
							  D3DXVECTOR2(originX + width, originY + height),
							  D3DXVECTOR2(originX, height + originY),
							  D3DXVECTOR2(originX, originY)};
	line->SetWidth(thickness);
	line->Begin();
	line->Draw(vertices, ARRAYSIZE(vertices), D3DCOLOR_ARGB(255, 255, 255, 255));
	line->End();
	line->Release();
}
//==================================================================================
void Renderer::endFrame(){
	m_pkDevice->EndScene();
	m_pkDevice->Present(NULL, NULL, NULL, NULL);
}
//==================================================================================
void Renderer::setMatrix(MatrixType matrixType, const Matrix& matrix){
	m_pkDevice->SetTransform(_matrixType[matrixType], matrix);
}
//==================================================================================
const Texture Renderer::loadTexture(const std::string& textureName, D3DCOLOR ColorKey){
	Texture texture = NULL;

	D3DXCreateTextureFromFileEx(m_pkDevice,
								textureName.c_str(),
								D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 0, 0,
								D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
								D3DX_FILTER_NONE, D3DX_FILTER_NONE,
								ColorKey,
								NULL,
								NULL,
								&texture);

	if(texture){
		for(size_t i = 0; i < _textureList.size(); i++){
			if(texture == _textureList[i])
				return texture;
		}

		_textureList.push_back(texture);
		return texture;
	}

	else{
		MessageBox(_hwnd,"No Texture", "No Texture",MB_OK);
		return NoTexture;
	}
}
//==================================================================================
void Renderer::setCurrentTexture(const Texture& texture){
	m_pkDevice->SetTexture(0, texture);
	m_pkDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pkDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
}
//==================================================================================
Matrix& Renderer::getProjectionMatrix(){
	return _projectionMatrix;
}
//==================================================================================
pg2::VertexBuffer* Renderer::createVertexBuffer(size_t vertexSize, unsigned int fvf){
	pg2::VertexBuffer* vertexBuffer;
	if (fvf == 0)
		vertexBuffer = new pg2::VertexBuffer(*this, m_pkDevice, vertexSize, TEXTUREFVF);
	else
		vertexBuffer = new pg2::VertexBuffer(*this, m_pkDevice, vertexSize, CUSTOMFVF);

	return vertexBuffer;
}
//==================================================================================
pg2::IndexBuffer* Renderer::createIndexBuffer(){
	pg2::IndexBuffer* indexBuffer = new pg2::IndexBuffer(*this, m_pkDevice);
	return indexBuffer;
}
//==================================================================================
void Renderer::setCurrentIndexBuffer(pg2::IndexBuffer* indexBuffer){
	_indexBuffer = indexBuffer;
}
//==================================================================================
void Renderer::setCurrentVertexBuffer(pg2::VertexBuffer* vertexBuffer){
	_vertexBuffer = vertexBuffer;
}
//==================================================================================
void Renderer::drawCurrentBuffers(Primitive primitive){
	m_pkDevice->DrawIndexedPrimitive(_primitives[primitive], 0, 0, _vertexBuffer->vertexCount(), 0, _indexBuffer->indexCount() / 3);
}
//==================================================================================
decomposedMatrix Renderer::decomposeMatrix(const float matrix[4][4]){
	D3DXMATRIX dx9Matrix;
	dx9Matrix._11 = matrix[0][0];
	dx9Matrix._12 = matrix[0][1];
	dx9Matrix._13 = matrix[0][2];
	dx9Matrix._14 = matrix[0][3];

	dx9Matrix._21 = matrix[1][0];
	dx9Matrix._22 = matrix[1][1];
	dx9Matrix._23 = matrix[1][2];
	dx9Matrix._24 = matrix[1][3];

	dx9Matrix._31 = matrix[2][0];
	dx9Matrix._32 = matrix[2][1];
	dx9Matrix._33 = matrix[2][2];
	dx9Matrix._34 = matrix[2][3];

	dx9Matrix._41 = matrix[3][0];
	dx9Matrix._42 = matrix[3][1];
	dx9Matrix._43 = matrix[3][2];
	dx9Matrix._44 = matrix[3][3];
	for (size_t i = 0; i < 4; i++){
		for (size_t j = 0; j < 4; j++)
			dx9Matrix.m[i][j] = matrix[i][j];
	}

	/*for (size_t i = 0; i < 4; i++){
		for (size_t j = 0; j < 4; j++)
			std::cout << " " << dx9Matrix.m[i][j];
		std::cout << std::endl;
	}*/
	D3DXVECTOR3 pos, scale;
	D3DXQUATERNION rot;
	D3DXMatrixDecompose(&scale, &rot, &pos, &dx9Matrix);

	decomposedMatrix decomposedMatrix;
	decomposedMatrix.posX = pos.x;
	decomposedMatrix.posY = pos.y;
	decomposedMatrix.posZ = pos.z;
	//std::cout << scale.x << " " << scale.y << " " << scale.z << std::endl;
	decomposedMatrix.scaleX = scale.x;
	decomposedMatrix.scaleY = scale.y;
	decomposedMatrix.scaleZ = scale.z;

	decomposedMatrix.rotX = rot.x;
	decomposedMatrix.rotY = rot.y;
	decomposedMatrix.rotZ = rot.z;

	return decomposedMatrix;
}
