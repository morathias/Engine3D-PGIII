#ifndef GRIDNODE_H
#define GRIDNODE_H

#include "Vector2.h"

enum Player{
	Circulo,
	Cruz,
	Ninguno
};

class GridNode
{
public:
	GridNode();
	~GridNode();

	void setPos(float x, float y);
	Vector2& getPos();

	bool estaOcupado();
	void seOcupo(Player player);

	Player ocupadoPor();

private:
	Vector2* _posicion;
	bool _estaOcupado;
	Player _ocupadoPor;
};

#endif

