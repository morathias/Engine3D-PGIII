#include "TaTeTiServer.h"

#include <iostream>

using namespace std;

bool TaTeTiServer::init(Renderer& renderer){
	_turn = 1;

	_camera = new Camera();

	Debuger::activate(false);

	_server = new UDPServer();

	_text = new ScreenText();
	_text->create(320, 200, 500, 200, 20, "Bodoni MT Black", "", false, renderer);

	if (!_server->init()){
		cout << "error while creating server" << endl;
		return false;
	}

	_server->setMaxClientsCount(2);

	_text->setText("Server is up, waiting for clients...");

	return true;
}

void TaTeTiServer::frame(Renderer& renderer, Input& input, pg1::Timer& timer){
	_camera->update(renderer);

	_text->display(renderer);

	string message = _server->startListeningData();

	if (message == "clicked"){
		if (_server->sendData(_turn))
			cout << "data sent correctly" << endl;

		if (_turn == 0)
			_turn = 1;

		else if (_turn == 1)
			_turn = 0;
	}

	else if (message != "false"){
		if(_server->sendData(_turn))
			cout << "data sent correctly" << endl;
	}
}

void TaTeTiServer::fixedFrame(Input& input){

}

void TaTeTiServer::deinit(){

}
