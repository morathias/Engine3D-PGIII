#ifndef UTILITY_H
#define UTILITY_H
//========================================================================================
#include <windows.h>
#include "DLLExport.h"
#include <string>
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
enum BspResult{
	Front,
	Back,
	Intersecting,
};
//========================================================================================
struct decomposedMatrix{
	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;
	float rotX, rotY, rotZ;
};
//========================================================================================
class Utility{
public:
	DllExport static bool containsWord(std::string wordToCheck, std::string keyWord){
		int index = 0;

		for (size_t i = 0; i < wordToCheck.size(); i++){
			if (index == keyWord.size())
				break;

			if (wordToCheck[i] == keyWord[index])
				index++;
			else
				index = 0;
		}

		if (index == keyWord.size())
			return true;

		return false;
	}
};
//========================================================================================
#endif