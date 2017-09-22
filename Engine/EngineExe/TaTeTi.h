#ifndef TATETI_H
#define TATETI_H

#include <math.h>

#include <vector>
#include <list>
#include "Game.h"
#include "GridNode.h"
#include "Jugador.h"

using namespace std;

class TaTeTi : public Game{
public:
	bool init(Renderer& renderer);

	void frame(Renderer& renderer, Input& input, pg1::Timer& timer);
	void fixedFrame(Input& input);

	void deinit();

private:
	Camera* camera;
	Nodo _root;
	Importer* _importer;
	Light* _light;

	GridNode _grilla[3][3];
	int x, y;

	Player _turn;

	Jugador _cruz;
	Jugador _circulo;
	int _scoreCruz;
	int _scoreCirculo;

	bool _start;
	bool _endGame;
	float _timer;

	bool checkRow();
	bool checkColumn();
	bool checkDiagonalLtoR();
	bool checkDiagonalRtoL();

	bool checkWinner();
};

#endif

