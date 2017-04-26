#ifndef PACMAN_H
#define PACMAN_H
//==================================================================================
#include <math.h>
#include <vector>
#include <list>
#include "Game.h"
//==================================================================================
using namespace std;
//==================================================================================
class Pacman : public Game{
public:
	bool init(Renderer& rkRenderer);

	void frame(Renderer& rkRenderer, Input& input, pg1::Timer& timer);
	void fixedFrame(Input& input);

	void deinit();

private:
	Camera* camera;
	Nodo _root;
	Importer* _importer;
	BspTree _bspTree;
};
//==================================================================================
#endif
