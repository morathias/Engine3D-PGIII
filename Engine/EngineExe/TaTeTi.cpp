#include "TaTeTi.h"
#include "input.h"
#include <iostream>

bool TaTeTi::init(Renderer& renderer){
	_server = NULL;
	_client = NULL;

	Debuger::activate(false);

	_states = GameStates::Playing;
	_timer = 0.2f;

	camera = new Camera();
	camera->setPos(0, 5.2f, -1.2f);
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

	_circulo.setMesh(_root.childs()[2]);
	_cruz.setMesh(_root.childs()[1]);
	
	x = 1;
	y = 1;

	_grilla[0][0].setPos(-1.23f, 1.23f); _grilla[0][1].setPos(0, 1.23f); _grilla[0][2].setPos(1.23f, 1.23f);
	_grilla[1][0].setPos(-1.23f, 0);     _grilla[1][1].setPos(0, 0);     _grilla[1][2].setPos(1.23f, 0);
	_grilla[2][0].setPos(-1.23f, -1.23f);_grilla[2][1].setPos(0, -1.23f);_grilla[2][2].setPos(1.23f, -1.23f);

	_turn = static_cast<Player>(Random::valueBetweenInts(0,1));

	_mesagge = new ScreenText();
	_mesagge->create(420, 200, 500, 200, 40, "Bodoni MT Black", "", false, renderer);

	string a;
	cout << "is this the server?" << endl << "y/n" << endl;
	cin >> a;

	if (a == "y"){
		_server = new UDPServer();

		if (!_server->init()){
			cout << "error while creating server" << endl;
			return false;
		}
	}

	else{
		_client = new UDPClient();

		if (!_client->init()){
			cout << "error while creating client" << endl;
			return false;
		}

		_client->registerToServer();
	}

	return true;
}

int full = 0;
void TaTeTi::frame(Renderer& renderer, Input& input, pg1::Timer& timer){
	camera->update(renderer);

	string ganador = "";

	switch (_states)
	{
	case Starting:
		//movePlayer(input, timer.timeBetweenFrames() / 1000);

		_timer -= timer.timeBetweenFrames() / 1000;

		if (_timer <= 0){

			if (_turn == Player::Circulo){
				_turn = Player::Cruz;
			}
			else if (_turn == Player::Cruz){
				_turn = Player::Circulo;
			}

			_states = GameStates::Playing;
			_timer = 0.2f;
		}
		break;

	case Playing:
		//cout << "Playing" << endl;
		_mesagge->setText("");

		movePlayer(input, timer.timeBetweenFrames() / 1000);

		if (input.mouseDown(input.MB_1) && !_grilla[y][x].estaOcupado()){

			if (_client != NULL){
				if (!_client->sendData("clicked"))
					cout << "error, data not sent" << endl;
			}

			if (_server != NULL){
				if (!_server->sendData())
					cout << "error, data not sent" << endl;
			}

			

			_grilla[y][x].seOcupo(_turn);

			placePlayer(renderer);

			_states = checkWinner();

			if (isGridFull() && _states != GameStates::Ended)
				_states = GameStates::Ended;
			else
				full = 0;

			x = 1;
			y = 1;
		}

		if (_client != NULL)
			_client->startListeningData();

		if (_server != NULL){
			if (!_server->startListeningData()){}
				//cout << "no data received" << endl;
		}

		break;

	case Ended:
		
		if (full == 9)
			ganador = "ninguno";

		else{
			if (_turn == Player::Circulo)
				ganador = "circulo";
			else
				ganador = "cruz";
		}

		_mesagge->setText("se termino el juego,\n el ganador es " + ganador);

		if (_client != NULL){
			if (!_client->sendData("se termino el juego,\n el ganador es " + ganador))
				cout << "error, data not sent" << endl;
		}
		break;

	default:
		break;
	}
	
	_mesagge->display(renderer);
	_root.updateBV();
	_root.draw(renderer, PartiallyInside, camera->getFrustum());
	
}

void TaTeTi::fixedFrame(Input& input){

}

void TaTeTi::deinit(){

}

bool TaTeTi::isGridFull(){
	full = 0;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (_grilla[i][j].estaOcupado())
				full++;
		}
	}

	if (full == 9)
		return true;

	return false;
}

void TaTeTi::placePlayer(Renderer& renderer){
	Mesh* meshToPlace = new Mesh(renderer);

	if (_turn == Player::Circulo){
		Nodo* nodo = (Nodo*)_root.childs()[2];
		Mesh* mesh = (Mesh*)nodo->childs()[0];

		meshToPlace->setMeshData(mesh->getVertices(), Primitive::TRIANGLELIST, mesh->getVertexCount(), mesh->getIndices(), mesh->getIndexCount());
		meshToPlace->setPosZ(0.0f);
		meshToPlace->setMaterial(mesh->getMaterial());
	}
	else if (_turn == Player::Cruz){
		Nodo* nodo = (Nodo*)_root.childs()[1];
		Mesh* mesh = (Mesh*)nodo->childs()[0];

		meshToPlace->setMeshData(mesh->getVertices(), Primitive::TRIANGLELIST, mesh->getVertexCount(), mesh->getIndices(), mesh->getIndexCount());
		meshToPlace->setScale(0.2f, 0.2f, 0.2f);
		meshToPlace->setPosZ(0.0f);
		meshToPlace->setMaterial(mesh->getMaterial());
	}

	meshToPlace->buildAABB();
	meshToPlace->setPosX(_grilla[y][x].getPos().x());
	meshToPlace->setPosY(_grilla[y][x].getPos().y());
	

	meshToPlace->setParent(&_root);

	_root.addChild(*meshToPlace);
}

void TaTeTi::movePlayer(Input& input, float dt){
	switch (_turn)
	{
	case Circulo:
		_circulo.setTargetPos(&_grilla[y][x].getPos());
		_circulo.stateMachine(input, x, y, dt, _client);
		break;
	case Cruz:
		_cruz.setTargetPos(&_grilla[y][x].getPos());
		_cruz.stateMachine(input, x, y, dt, _client);
		break;
	case Ninguno:
		break;
	default:
		break;
	}
}

GameStates TaTeTi::checkRow(){
	size_t row = 0;
	for (row = 0; row < 3; row++){
		if (_grilla[y][row].ocupadoPor() != _turn)
			return GameStates::Starting;
	}

	cout << "fila ganadora" << endl;
	return GameStates::Ended;
}

GameStates TaTeTi::checkColumn(){
	for (size_t column = 0; column < 3; column++){
		if (_grilla[column][x].ocupadoPor() != _turn)
			return GameStates::Starting;
	}

	cout << "columna ganadora" << endl;
	return GameStates::Ended;
}

GameStates TaTeTi::checkDiagonalLtoR(){
	int row = 0,column = 0;

	while (row < 3){
		if (_grilla[column][row].ocupadoPor() != _turn)
			return GameStates::Starting;

		row++;
		column++;
	}

	cout << "diagonal L ganadora" << endl;

	return GameStates::Ended;
}

GameStates TaTeTi::checkDiagonalRtoL(){
	int row = 2, column = 0;

	while (column < 3){
		if (_grilla[column][row].ocupadoPor() != _turn)
			return GameStates::Starting;

		row--;
		column++;
	}

	cout << "diagonal R ganadora" << endl;

	return GameStates::Ended;
}

GameStates TaTeTi::checkWinner(){
	if (checkRow() == GameStates::Ended)
		return GameStates::Ended;

	if (checkColumn())
		return GameStates::Ended;

	if (checkDiagonalLtoR())
		return GameStates::Ended;

	if (checkDiagonalRtoL())
		return GameStates::Ended;

	return GameStates::Starting;
}