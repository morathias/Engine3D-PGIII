#ifndef DEBUGER_H
#define DEBUGER_H
//================================================================
#include "Renderer.h"
#include "Nodo.h"
#include "pg1_timer.h"
#include "Line.h"
#include "ScreenText.h"
#include <string>
//================================================================
using namespace std;
//================================================================
class Debuger
{
public:
	Debuger(Renderer& renderer, pg1::Timer& timer);
	~Debuger();

	static void setScene(Nodo& root);
	bool initScene();
	void updateScene();

	static bool boundingBoxesShown();
	static void getBatches();

public:
	DllExport static void activate();
	DllExport static void showSceneGraph(bool show);
	DllExport static void showBoundignBox(bool show);
	DllExport static void showBspTree(bool show);

	DllExport static void createDebugText();
	DllExport static void setDebugText(int index, string text);

private:
	Renderer& _renderer;
	pg1::Timer& _timer;
	static Nodo _root;

	static bool _activated;
	static bool _showSceneGraph;
	static bool _showBoundingBox;
	static bool _showBspTree;

	vector<string> _names;
	vector<ScreenText*> _namesTxt;

	Line _sceneGraphRect;

	int _vertexCount;
	ScreenText* _vertexCountTxt;

	int _triangleCount;
	ScreenText* _triangleCountTxt;

	static int _batches;
	ScreenText* _batchesTxt;

	int _fps;
	ScreenText* _fpsTxt;

	Line _sceneStatsRect;

	static vector<ScreenText*> _debugTxt;
};
//================================================================
#endif

