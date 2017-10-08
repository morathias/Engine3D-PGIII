#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "DLLExport.h"
//#include "UDPClient.h"
#include <string>
#include <map>

using namespace std;

struct Client;

class UDPServer
{
public:
	DllExport UDPServer();
	DllExport ~UDPServer();

	DllExport bool init();
	DllExport string startListeningData();
	DllExport bool sendData();
	DllExport bool sendData(string data);
	DllExport bool sendData(int clientIndex);

	DllExport void setMaxClientsCount(int maxClientCount);
	DllExport int getMaxClientCount() const;
	//DllExport void addClient(UDPClient& client);

private:
	int _socketLength, _recievedLength;

	char *buffer;

	int _clientCount, _maxClientCount;
	map<unsigned int, Client*> _clients;
};

#endif
