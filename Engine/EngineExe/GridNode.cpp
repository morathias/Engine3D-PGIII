#include "GridNode.h"


GridNode::GridNode():
_estaOcupado(false),
_ocupadoPor(Player::Ninguno)
{}

GridNode::~GridNode(){
	delete _posicion;
}

void GridNode::setPos(float x, float y){
	_posicion = new Vector2(x, y);
}
Vector2& GridNode::getPos(){
	return *_posicion;
}

bool GridNode::estaOcupado(){
	return _estaOcupado;
}
void GridNode::seOcupo(Player player){
	_ocupadoPor = player;
	_estaOcupado = true;
}

Player GridNode::ocupadoPor(){
	return _ocupadoPor;
}