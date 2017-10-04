#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "DLLExport.h"

#include <string>

using namespace std;

class UDPClient
{
public:
	DllExport UDPClient();
	DllExport ~UDPClient();

	DllExport bool init();
	DllExport bool startListeningData();
	DllExport bool sendData(string data);

	DllExport bool registerToServer();

private:
	int _socket, _socketLength;

	char *_buffer;
};

#endif

