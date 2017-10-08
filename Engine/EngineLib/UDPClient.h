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
	DllExport string startListeningData();
	DllExport bool sendData(string data);

	DllExport bool registerToServer();
	DllExport int getId() const;

private:
	int _socket, _socketLength;

	int _id;

	char *_buffer;
};

#endif

