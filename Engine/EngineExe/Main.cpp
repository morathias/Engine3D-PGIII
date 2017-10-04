#include "engine.h"
#include "pacman.h"
#include "TaTeTi.h"
//===============================================================================================
using namespace std;
//===============================================================================================
static const unsigned int width = 640;
static const unsigned int heigth = 480;
//===============================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Engine eng(hInstance, width, heigth);

	Game* a = new TaTeTi();
	eng.setGame(a);

	if (!eng.init()) return 1;

	eng.run();

	return 0;
}
//===============================================================================================