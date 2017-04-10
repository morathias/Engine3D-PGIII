#include "Debuger.h"
//================================================================
Debuger::Debuger(Renderer& renderer, pg1::Timer& timer) :
_renderer(renderer),
_timer(timer)
{}
//================================================================
Debuger::~Debuger(){
	if (!_namesTxt.empty()){
		for (size_t i = 0; i < _namesTxt.size(); i++){
			delete _namesTxt[i];
		}
	}

	delete _fpsTxt;
	delete _batchesTxt;
	delete _vertexCountTxt;
	delete _triangleCountTxt;
}
//================================================================
Nodo Debuger::_root;
void Debuger::setScene(Nodo& root){
	_root = root;
}
//================================================================
bool Debuger::initScene(){
	_fpsTxt = new ScreenText();
	_fpsTxt->create(50, 12, 100, 16, 15, "arial", "", true, _renderer);

	_batchesTxt = new ScreenText();
	_batchesTxt->create(150, 12, 100, 16, 15, "arial", "", true, _renderer);

	_vertexCountTxt = new ScreenText();
	_vertexCountTxt->create(50, 36, 100, 30, 15, "arial", "", true, _renderer);

	_triangleCountTxt = new ScreenText();
	_triangleCountTxt->create(150, 36, 100, 30, 15, "arial", "", true, _renderer);

	int lvl = 0;
	std::vector<int> lvlDeep;
	_root.getNames(_names, lvlDeep, lvl);

	int posX = 30, posY = 66;
	for (size_t i = 0; i < _names.size(); i++){
		ScreenText* nameTxt = new ScreenText();
		nameTxt->create(posX + lvlDeep[i] * 20, posY, 100, 16, 15, "arial", "", true, _renderer);
		_namesTxt.push_back(nameTxt);
		_namesTxt[i]->setText(_names[i]);
		
		posY += 16;
	}

	return true;
}
//================================================================
void Debuger::updateScene(){
	if (!_activated)
		return;

	string fpsTxt = "FPS: " + to_string(_timer.fps());
	_fpsTxt->setText(fpsTxt);
	_fpsTxt->display(_renderer);

	string batchesTxt = "Batches: " + to_string(_batches);
	_batchesTxt->setText(batchesTxt);
	_batchesTxt->display(_renderer);

	int vertexCount = 0;
	_root.updatePolygons(vertexCount);

	string vertexCountTxt = "Vertex Count: \n" + to_string(vertexCount);
	_vertexCountTxt->setText(vertexCountTxt);
	_vertexCountTxt->display(_renderer);

	vertexCountTxt = "Triangle Count: \n" + to_string(vertexCount / 3);
	_triangleCountTxt->setText(vertexCountTxt);
	_triangleCountTxt->display(_renderer);

	_sceneStatsRect.drawRect(0,0, 200, 52, 1.1f, _renderer);


	int index = 0;
	_root.updateNames(_names, index);
	for (size_t i = 0; i < _namesTxt.size(); i++){
		_namesTxt[i]->setText(_names[i]);
		_namesTxt[i]->display(_renderer);
	}

	_sceneGraphRect.drawRect(0, 54, 
							 200, _namesTxt[_namesTxt.size() - 1]->Y() - 40,
							 1.2,
							 _renderer);

	_batches = 0;
}
//================================================================
bool Debuger::_activated = true;
void Debuger::activate(){
	_activated = true;
}
//================================================================
bool Debuger::_showBoundingBox = false;
void Debuger::showBoundignBox(bool show){
	_showBoundingBox = show;
}
//================================================================
bool Debuger::boundingBoxesShown(){
	return _showBoundingBox;
}
//================================================================
bool Debuger::_showSceneGraph = true;
void Debuger::showSceneGraph(bool show){
	_showSceneGraph = show;
}
//================================================================
bool Debuger::_showBspTree = true;
void Debuger::showBspTree(bool show){
	_showBspTree = show;
}
//================================================================
int Debuger::_batches;
void Debuger::getBatches(){
	_batches++;
}
//================================================================