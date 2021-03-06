#ifndef GAME_H
#define GAME_H
//========================================================================================
#include "Camera.h"
#include "Quad.h"
#include "Sprite.h"
#include "Random.h"
#include "Vector2.h"
#include "ScreenText.h"
#include "Line.h"
#include "Sound.h"
#include "Mesh.h"
#include "Utility.h"
#include "Importer.h"
#include "Nodo.h"
#include "BspTree.h"
#include "Light.h"
#include "Material.h"
#include "Debuger.h"
//========================================================================================
class Input;
//========================================================================================
class Game{
public:
	DllExport Game();
	DllExport virtual bool init(Renderer& rkRenderer) = 0;
	DllExport virtual void frame(Renderer& rkRenderer, Input& rkInput, pg1::Timer& timer) = 0;
	DllExport virtual void fixedFrame(Input& input) = 0;
	DllExport virtual void deinit() = 0;
	DllExport bool isDone() const;
	DllExport void setDone(bool bDone);

private:
	bool m_bDone;
};
//========================================================================================
#endif