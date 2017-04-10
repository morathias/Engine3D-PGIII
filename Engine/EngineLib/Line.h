#ifndef LINE_H
#define LINE_H

#include "Renderer.h"

class Line{
public:
	DllExport Line();
	DllExport ~Line();

	DllExport void drawLine(float origin[2], float end[2], float thickness,Renderer& renderer);
	DllExport void drawRect(float originX, float originY, float width, float height, float thickness, Renderer& renderer);
};
#endif
