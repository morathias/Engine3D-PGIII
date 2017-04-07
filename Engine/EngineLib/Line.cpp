#include "Line.h"

Line::Line(){}

Line::~Line(){}

void Line::drawLine(float origin[2], float end[2], float thickness,Renderer& renderer){
	renderer.drawLine(origin, end, thickness);
}

void Line::drawRect(float origin, float width, float height, float thickness, Renderer& renderer){
	renderer.drawRect(origin, width, height, thickness);
}
