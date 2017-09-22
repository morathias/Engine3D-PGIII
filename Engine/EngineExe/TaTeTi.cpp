#include "TaTeTi.h"
#include "input.h"
#include <iostream>

bool TaTeTi::init(Renderer& renderer){
	_timer = 0.3f;
	_start = true;
	_endGame = false;

	camera = new Camera();
	camera->setPos(0, 5.2, -1.2);
	camera->pitch(1.35);

	_importer = new Importer(renderer);
	if (!_importer->importScene("Assets/tateti.dae", _root)){
		cout << "no se cargo escena";
		return false;
	}

	_light = new Light(0);
	_light->create();
	renderer.addLight(*_light);
	renderer.enableLight(true, *_light);

	_circulo.setMesh(_root.childs()[1]);
	_cruz.setMesh(_root.childs()[2]);
	
	x = 1;
	y = 1;

	_grilla[0][0].setPos(-1.23f, 1.23f); _grilla[0][1].setPos(0, 1.23f); _grilla[0][2].setPos(1.23f, 1.23f);
	_grilla[1][0].setPos(-1.23f, 0);     _grilla[1][1].setPos(0, 0);     _grilla[1][2].setPos(1.23f, 0);
	_grilla[2][0].setPos(-1.23f, -1.23f);_grilla[2][1].setPos(0, -1.23f);_grilla[2][2].setPos(1.23f, -1.23f);

	_turn = static_cast<Player>(Random::valueBetweenInts(0,1));

	return true;
}
int full = 0;
void TaTeTi::frame(Renderer& renderer, Input& input, pg1::Timer& timer){
	camera->update(renderer);

	switch (_turn)
	{
	case Circulo:
		_circulo.setTargetPos(&_grilla[y][x].getPos());
		_circulo.stateMachine(input, x, y, timer.timeBetweenFrames() / 1000);
		break;
	case Cruz:
		_cruz.setTargetPos(&_grilla[y][x].getPos());
		_cruz.stateMachine(input, x, y, timer.timeBetweenFrames() / 1000);
		break;
	case Ninguno:
		break;
	default:
		break;
	}
	
	
	if (input.mouseDown(input.MB_1) && _start && !_grilla[y][x].estaOcupado()){
		_start = false;

		_grilla[y][x].seOcupo(_turn);

		_endGame = checkWinner();

		full = 0;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				if (_grilla[i][j].estaOcupado())
					full++;
			}
		}

		if (_turn == Player::Circulo && !_endGame){
			_turn = Player::Cruz;
		}
		else if (_turn == Player::Cruz && !_endGame){
			_turn = Player::Circulo;
		}

		x = 1;
		y = 1;
	}

	if (!_start){
		_timer -= timer.timeBetweenFrames() / 1000;

		if (_timer <= 0){
			_start = true;
			_timer = 0.3f;
		}
	}

	if (_endGame){
		string ganador = "";
		if (full == 9)
			ganador = "ninguno";

		else{
			if (_turn == Player::Circulo)
				ganador = "circulo";
			else
				ganador = "cruz";
		}
		
			

		cout << "se termino el juego, el ganador es " <<ganador<< endl;
	}

	_root.updateBV();
	_root.draw(renderer, PartiallyInside, camera->getFrustum());
}

void TaTeTi::fixedFrame(Input& input){

}

void TaTeTi::deinit(){

}

bool TaTeTi::checkRow(){
	size_t row = 0;
	for (row = 0; row < 3; row++){
		if (_grilla[y][row].ocupadoPor() != _turn)
			return false;
	}

	cout << "fila ganadora" << endl;
	return true;
}

bool TaTeTi::checkColumn(){
	for (size_t column = 0; column < 3; column++){
		if (_grilla[column][x].ocupadoPor() != _turn)
			return false;
	}

	cout << "columna ganadora" << endl;
	return true;
}

bool TaTeTi::checkDiagonalLtoR(){
	int row = 0,column = 0;

	while (row < 3){
		if (_grilla[column][row].ocupadoPor() != _turn)
			return false;

		row++;
		column++;
	}

	cout << "diagonal L ganadora" << endl;

	return true;
}

bool TaTeTi::checkDiagonalRtoL(){
	int row = 2, column = 0;

	while (column < 3){
		if (_grilla[column][row].ocupadoPor() != _turn)
			return false;

		row--;
		column++;
	}

	cout << "diagonal R ganadora" << endl;

	return true;
}

bool TaTeTi::checkWinner(){
	if (checkRow())
		return true;

	if (checkColumn())
		return true;

	if (checkDiagonalLtoR())
		return true;

	if (checkDiagonalRtoL())
		return true;

	return false;
}