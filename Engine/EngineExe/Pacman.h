#ifndef PACMAN_H
#define PACMAN_H
//==================================================================================
#include <math.h>
#include <vector>
#include <list>
#include "Game.h"
#include "TileMap.h"
//==================================================================================
using namespace std;
//==================================================================================
class Pacman : public Game{
public:
	bool init(Renderer& rkRenderer);
	void frame(Renderer& rkRenderer, Input& input, pg1::Timer& timer);
	void deinit();

	void moveRoot(Input& input);
	void moveNode1(Input& input);
	void moveMesh(Input& input);

private:
	Camera* camera;
	Nodo _root;

	Importer* _importer;
	Mesh* _max; Mesh* _min;

	ScreenText* _screenText;
	list <string> meshNames;
	vector <string> names;
	string _text;
};
//==================================================================================
#endif
