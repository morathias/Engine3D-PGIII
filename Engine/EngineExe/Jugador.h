#ifndef JUGADOR_H
#define JUGADOR_H

#include "Entity3D.h"
#include "input.h"
#include "UDPClient.h"

class Jugador{

	enum estados{
		esparando,
		moviendose
	};

public:
	Jugador();
	~Jugador();

	void stateMachine(Input& input, int& x, int& y, float dt, UDPClient* client);

	void setTargetPos(Vector2* targetPos);

	void setMesh(Entity3D* mesh);
	Entity3D& getMesh();

	void setTurn(int turn);
	int getTurn();

private:
	Entity3D* _mesh;
	estados _estado;

	Vector2* _targetPos;

	float _timer;
	
	int _turn;
};

#endif

