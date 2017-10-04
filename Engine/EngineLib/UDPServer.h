#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "DLLExport.h"
//#include "UDPClient.h"
#include <string>
#include <map>

using namespace std;

class UDPServer
{
public:
	DllExport UDPServer();
	DllExport ~UDPServer();

	DllExport bool init();
	DllExport bool startListeningData();
	DllExport bool sendData();
	DllExport bool sendData(int clientIndex);

	DllExport void setMaxClientsCount(int maxClientCount);
	DllExport int getMaxClientCount() const;
	//DllExport void addClient(UDPClient& client);

private:
	int _socketLength, _recievedLength;

	char *buffer;

	int _clientCount, _maxClientCount;
	map<unsigned int, unsigned int> _clients;
};

#endif
