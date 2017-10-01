#ifndef TATETI_H
#define TATETI_H

#include <math.h>

#include <vector>
#include <list>
#include "Game.h"
#include "GridNode.h"
#include "Jugador.h"

using namespace std;

enum GameStates{
	Starting,
	Playing,
	Ended
};

class TaTeTi : public Game{
public:
	bool init(Renderer& renderer);

	void frame(Renderer& renderer, Input& input, pg1::Timer& timer);
	void fixedFrame(Input& input);

	void deinit();

private:
	GameStates _states;

	Camera* camera;
	Nodo _root;
	Importer* _importer;
	Light* _light;

	GridNode _grilla[3][3];
	int x, y;

	Player _turn;

	ScreenText* _mesagge;

	Jugador _cruz;
	Jugador _circulo;
	int _scoreCruz;
	int _scoreCirculo;

	bool _start;
	bool _endGame;
	float _timer;

	void placePlayer(Renderer& renderer);

	void movePlayer(Input& input, float dt);

	bool isGridFull();

	GameStates checkRow();
	GameStates checkColumn();
	GameStates checkDiagonalLtoR();
	GameStates checkDiagonalRtoL();

	GameStates checkWinner();
};

#endif

