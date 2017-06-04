#ifndef LIGHT_H
#define LIGHT_H

#include "DLLExport.h"

struct _D3DLIGHT9;
typedef _D3DLIGHT9 D3DLIGHT9;

class Light{
public:
	DllExport Light(int index);
	DllExport ~Light();

	DllExport void create();
	DllExport int getIndex() const;

	DllExport D3DLIGHT9* getD3dLight() const;

private:
	int _index;

	D3DLIGHT9* _d3dLight;
};

#endif
