#ifndef SCREENTEXT_H
#define SCREENTEXT_H
//===================================================================
#include <string>
#include "Renderer.h"
//===================================================================
class ScreenText
{
public:
	DllExport ScreenText();
	DllExport ~ScreenText();

	DllExport void create(int x, int y, 
						  int width, int height,
						  int charSize,
						  std::string textFont, std::string text,
						  bool italic,
						  Renderer& renderer);

	DllExport void display(Renderer& renderer);

	DllExport void setText(std::string text);

	DllExport void setX(int x);
	DllExport int X() const;

	DllExport void setY(int y);
	DllExport int Y() const;

	DllExport void setWidth(int width);
	DllExport int width() const;

	DllExport void setHeight(int height);
	DllExport int height() const;

private:
	void updateRect();

private:
	Font _font;
	int _width, _height;
	int _charSize;
	int _x, _y;
	bool _italic;
	std::string _textFont;
	RECT _rect;
	std::string _text;
};
//===================================================================
#endif

