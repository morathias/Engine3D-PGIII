#include "TaTeTiServer.h"

#include <iostream>

using namespace std;

bool TaTeTiServer::init(Renderer& renderer){
	_camera = new Camera();

	Debuger::activate(false);

	_server = new UDPServer();

	_text = new ScreenText();
	_text->create(320, 200, 500, 200, 20, "Bodoni MT Black", "", false, renderer);

	if (!_server->init()){
		cout << "error while creating server" << endl;
		return false;
	}

	_text->setText("Server is up, waiting for clients...");

	return true;
}

void TaTeTiServer::frame(Renderer& renderer, Input& input, pg1::Timer& timer){
	_camera->update(renderer);

	_text->display(renderer);

	_server->startListeningData();
}

void TaTeTiServer::fixedFrame(Input& input){

}

void TaTeTiServer::deinit(){

}
