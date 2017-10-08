#include "Rival.h"


#include <iostream>

using namespace std;

Rival::Rival(){
	_estado = estados::esparando;
	_timer = 0.02f;
}

Rival::~Rival(){}

void Rival::stateMachine(Input& input, int& x, int& y, float dt, string packet){
	switch (_estado)
	{
	case Rival::esparando:
		//cout << "Esperando" << endl;
		if (packet == "move right"){
			if (x != 2)
				x++;

			_estado = estados::moviendose;
		}
		if (packet == "move left"){
			if (x != 0)
				x--;

			_estado = estados::moviendose;
		}

		if (packet == "move down"){
			if (y != 2)
				y++;

			_estado = estados::moviendose;
		}
		if (packet == "move up"){
			if (y != 0)
				y--;

			_estado = estados::moviendose;
		}
		break;

	case Rival::moviendose:
		_timer -= dt;

		//cout << dt<<endl;

		if (_timer <= 0){
			cout << "rival llego" << endl;
			_timer = 0.02f;
			_estado = estados::esparando;
		}
		break;

	default:
		break;
	}

	_mesh->setPosX(Utility::lerp(_mesh->posX(), _targetPos->x(), 10.0f * dt));
	_mesh->setPosY(Utility::lerp(_mesh->posY(), _targetPos->y(), 10.0f * dt));
}

void Rival::setTargetPos(Vector2* targetPos){
	_targetPos = targetPos;
}

void Rival::setMesh(Entity3D* mesh){
	_mesh = mesh;
	_mesh->setPosZ(-0.8f);
}

Entity3D& Rival::getMesh(){
	return *_mesh;
}

void Rival::setTurn(int turn){
	_turn = turn;
}
int Rival::getTurn(){
	return _turn;
}
