#ifndef TATETISERVER_H
#define TATETISERVER_H

#include "UDPServer.h"
#include "Game.h"

class TaTeTiServer : public Game
{
public:
	bool init(Renderer& renderer);

	void frame(Renderer& renderer, Input& input, pg1::Timer& timer);
	void fixedFrame(Input& input);

	void deinit();

private :
	Camera *_camera;

	UDPServer *_server;

	ScreenText *_text;

};

#endif

