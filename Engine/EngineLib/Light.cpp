#include "Light.h"

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib") 
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

Light::Light(int index){
	_index = index;
}

Light::~Light(){
	delete _d3dLight;
}

void Light::create(){
	_d3dLight = new D3DLIGHT9();

	ZeroMemory(_d3dLight, sizeof(_d3dLight));

	_d3dLight->Type = D3DLIGHT_DIRECTIONAL;
	_d3dLight->Diffuse = D3DXCOLOR(1.0f, 0.97f, 0.74f, 1.0f);
	_d3dLight->Direction = D3DXVECTOR3(1.0f, -0.5f, 1.0f);
	_d3dLight->Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	/*_d3dLight->Range = 1000;
	_d3dLight->Falloff = 0;
	_d3dLight->Attenuation0 = 1;
	_d3dLight->Attenuation1 = 0;
	_d3dLight->Attenuation2 = 0;*/
}

int Light::getIndex() const{
	return _index;
}

D3DLIGHT9* Light::getD3dLight() const{
	return _d3dLight;
}