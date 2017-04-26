#include "Pacman.h"
#include "input.h"
#include <iostream>
//==================================================================================
bool Pacman::init(Renderer& rkRenderer){	
	camera = new Camera();
	camera->setPos(0, 0, -10);

	_importer = new Importer(rkRenderer);
	if (!_importer->importScene("Assets/untitled.dae", _root))
		cout << "no se cargo escena";

	_bspTree.buildTree();

	Debuger::showBoundignBox(true);
	return true;
}
//==================================================================================
bool shown = true;
void Pacman::frame(Renderer& rkRenderer, Input& input, pg1::Timer& timer){
	//Movimiento de la camara
	if (input.keyDown(Input::KEY_H)){
		camera->strafe(10.0f * (timer.timeBetweenFrames() / 1000.0f));
	}
	else if (input.keyDown(Input::KEY_F)){
		camera->strafe(-10.0f * (timer.timeBetweenFrames() / 1000.0f));
	}

	if (input.keyDown(Input::KEY_T)){
		camera->walk(10.0f * (timer.timeBetweenFrames() / 1000.0f));
	}
	else if (input.keyDown(Input::KEY_G)) {
		camera->walk(-10.0f * (timer.timeBetweenFrames() / 1000.0f));
	}

	camera->yaw(input.mouseRelPosX() * 0.005f);
	camera->pitch(input.mouseRelPosY() * 0.005f);
	camera->roll(input.mouseRelPosZ() * 0.0005f);

	camera->update(rkRenderer);

	if (input.keyDown(Input::KEY_F4)){
		if (shown) shown = false;
		else shown = true;
	}
	Debuger::showBoundignBox(shown);

	_root.updateBV();
	_root.draw(rkRenderer, PartiallyInside, camera->getFrustum());
}
//==================================================================================
void Pacman::fixedFrame(Input& input){
	//_root.updatePhysics();
}

void Pacman::deinit(){
	delete camera;
	delete _importer;
}
//==================================================================================