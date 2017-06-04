#include "Material.h"

#include <d3d9.h>
#pragma comment (lib, "d3d9.lib") 
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")

Material::Material():
_d3dMaterial(new D3DMATERIAL9())
{}

Material::~Material(){
	delete _d3dMaterial;
}

D3DMATERIAL* Material::getD3dMaterial() const{
	return _d3dMaterial;
}

void Material::setColor(float r, float g, float b, float a){
	_d3dMaterial->Diffuse = D3DXCOLOR(r, g, b, a);
}

void Material::setAmbient(float r, float g, float b, float a){
	_d3dMaterial->Ambient = D3DXCOLOR(r, g, b, a);
}

void Material::setSpecular(float r, float g, float b){
	_d3dMaterial->Specular = D3DXCOLOR(r, g, b, 1.0f);
}

void Material::setSpecularSharpness(float sharpness){
	_d3dMaterial->Power = sharpness;
}
