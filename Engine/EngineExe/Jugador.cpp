#include "Jugador.h"
#include <iostream>

using namespace std;

Jugador::Jugador(){
	_estado = estados::esparando;
	_timer = 0.2f;
}

Jugador::~Jugador(){}

void Jugador::stateMachine(Input& input, int& x, int& y, float dt, UDPClient* client){
	switch (_estado)
	{
	case Jugador::esparando:

		//cout << "Esperando" << endl;
		if (input.keyDown(input.KEY_D)){
			if (x != 2)
				x++;
			if (client != NULL){
				if (!client->sendData("move right"))
					cout << "error, data not sent" << endl;
			}
			_estado = estados::moviendose;
		}
		if (input.keyDown(input.KEY_A)){
			if (x != 0)
				x--;
			if (client != NULL){
				if (!client->sendData("move left"))
					cout << "error, data not sent" << endl;
			}
			_estado = estados::moviendose;
		}

		if (input.keyDown(input.KEY_S)){
			if (y != 2)
				y++;
			if (client != NULL){
				if (!client->sendData("move down"))
					cout << "error, data not sent" << endl;
			}
			_estado = estados::moviendose;
		}
		if (input.keyDown(input.KEY_W)){
			if (y != 0)
				y--;
			if (client != NULL){
				if (!client->sendData("move up"))
					cout << "error, data not sent" << endl;
			}
			_estado = estados::moviendose;
		}
		break;

	case Jugador::moviendose:
		_timer -= dt;

		//cout << dt<<endl;

		if (_timer <= 0){
			//cout << "llego" << endl;
			_timer = 0.2f;
			_estado = estados::esparando;
		}
		break;

	default:
		break;
	}

	_mesh->setPosX(Utility::lerp(_mesh->posX(), _targetPos->x(), 10.0f * dt));
	_mesh->setPosY(Utility::lerp(_mesh->posY(), _targetPos->y(), 10.0f * dt));
}

void Jugador::setTargetPos(Vector2* targetPos){
	_targetPos = targetPos;
}

void Jugador::setMesh(Entity3D* mesh){
	_mesh = mesh;
	_mesh->setPosZ(-0.8f);
}
Entity3D& Jugador::getMesh(){
	return *_mesh;
}