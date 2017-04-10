#include "ScreenText.h"
//=================================================
ScreenText::ScreenText():
	_font(NULL),
	_width(0),
	_height(0),
	_x(0),
	_y(0),
	_charSize(0),
	_italic(false)
{}
//=================================================
ScreenText::~ScreenText(){
}
//=================================================
void ScreenText::create(int x, int y, 
						int width, int height,
						int charSize,
						std::string textFont, std::string text,
						bool italic,
						Renderer& renderer)
{
	_font = renderer.createFont(charSize, textFont, italic);
	_rect = renderer.createRect(x, y, width, height);

	_text = text;

	_x = x;
	_y = y;

	_width = width;
	_height = height;
}
//=================================================
void ScreenText::display(Renderer& renderer){
	renderer.displayText(_font, _rect, _text);
}
//=================================================
void ScreenText::setText(std::string text){
	_text = text;
}
//=================================================
void ScreenText::setX(int x){
	_x = x;
	updateRect();
}
int ScreenText::X() const{
	return _x;
}
//=================================================
void ScreenText::setY(int y){
	_y = y;
	updateRect();
}
int ScreenText::Y() const{
	return _y;
}
//=================================================
void ScreenText::setWidth(int width){
	_width = width;
}
int ScreenText::width() const{
	return _width;
}
//=================================================
void ScreenText::setHeight(int height){
	_height = height;
}
int ScreenText::height() const{
	return _height;
}
//=================================================
void ScreenText::updateRect(){
	_rect.left = _x - (_width / 2);
	_rect.top = _y - (_height / 2);
	_rect.right = _x + (_width / 2);
	_rect.bottom = _y + (_height / 2);
}
//=================================================