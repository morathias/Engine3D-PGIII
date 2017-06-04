#ifndef MATERIAL_H
#define MATERIAL_H

#include "DLLExport.h"

struct _D3DMATERIAL9;
typedef _D3DMATERIAL9 D3DMATERIAL;

class Material{
public:
	DllExport Material();
	DllExport ~Material();

	DllExport D3DMATERIAL* getD3dMaterial() const;

	DllExport void setColor(float r, float g, float b, float a);
	DllExport void setAmbient(float r, float g, float b, float a);
	DllExport void setSpecular(float r, float g, float b);
	DllExport void setSpecularSharpness(float sharpness);

private:
	D3DMATERIAL* _d3dMaterial;
};

#endif

