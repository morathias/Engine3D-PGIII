#ifndef UTILITY_H
#define UTILITY_H
//========================================================================================
#include <windows.h>
//========================================================================================
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0]))) //dinamic array size
//========================================================================================
struct Vertex{
	float x, y, z;
	DWORD color;
};
//========================================================================================
struct TexturedVertex{
	float x, y, z;
	float u, v;
};
//========================================================================================
enum Primitive{
	POINTLIST = 0,
	LINELIST,
	LINESTRIP,
	TRIANGLELIST,
	TRIANGLESTRIP,
	TRIANGLEFAN,
	PRIMITIVE_COUNT
};
//========================================================================================
enum MatrixType{
	VIEW,
	PROJECTION,
	WORLD,
	MATRIX_TYPE_COUNT
};
//========================================================================================
enum CollisionResult{
	AllInside,
	AllOutside,
	PartiallyInside
};
//========================================================================================
struct decomposedMatrix{
	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;
	float rotX, rotY, rotZ;
};
//========================================================================================

#endif