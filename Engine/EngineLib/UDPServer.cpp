#include "UDPServer.h"

#include <winsock2.h>
#include <WS2tcpip.h>
#include<stdio.h>

#include "Utility.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

struct Client{
	char* addres;
	USHORT port;
};

namespace UDP{
	unsigned int _socket;
	struct sockaddr_in _server, si_other;
}

UDPServer::UDPServer(){
	buffer = new char[BUFLEN];
	_clientCount = 0;
}

UDPServer::~UDPServer(){
	closesocket(UDP::_socket);
	WSACleanup();

	if (buffer != NULL)
		delete[] buffer;
}

bool UDPServer::init(){
	_socketLength = sizeof(UDP::si_other);

	WSADATA wsa;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return false;
	}
	printf("Initialised.\n");

	if ((UDP::_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		return false;
	}
	printf("Socket created.\n");

	UDP::_server.sin_family = AF_INET;
	char* serverAddres = "190.12.106.242";
	UDP::_server.sin_addr.s_addr = INADDR_ANY;
	UDP::_server.sin_port = htons(PORT);

	//make socket to non blocking mode
	u_long mode = 1;
	ioctlsocket(UDP::_socket, FIONBIO, &mode);

	//Bind
	if (bind(UDP::_socket, (struct sockaddr *)&UDP::_server, sizeof(UDP::_server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		return false;
	}
	puts("Bind done");

	printf("server address: %s", inet_ntoa(UDP::_server.sin_addr));

	return true;
}

string UDPServer::startListeningData(){
	//printf("Waiting for data...");
	//fflush(stdout);

	//clear the buffer by filling null, it might have previously received data
	memset(buffer, '\0', BUFLEN);

	//try to receive some data, this is a blocking call
	if ((_recievedLength = recvfrom(UDP::_socket, buffer, BUFLEN, 0, (struct sockaddr *) &UDP::si_other, &_socketLength)) == SOCKET_ERROR)
	{
		//printf("recvfrom() %i failed with error code : %d", _recievedLength, WSAGetLastError());
		return "false";
	}

	//print details of the client/peer and the data received
	printf("Received packet from %s:%d\n", inet_ntoa(UDP::si_other.sin_addr), ntohs(UDP::si_other.sin_port));
	printf("Data: %s\n", buffer);


	bool registerClient = Utility::containsWord(buffer, "REGISTER");
	if ( registerClient){
		Client* clientInfo = new Client();

		char* clientAddress = new char[32];
		strcpy(clientAddress, inet_ntoa(UDP::si_other.sin_addr));

		clientInfo->addres = clientAddress;
		clientInfo->port = UDP::si_other.sin_port;

		_clients.insert(pair<unsigned int, Client*>(_clientCount, clientInfo));

		printf("client: %i added, ip: %s", _clientCount, _clients[_clientCount]->addres);

		_clientCount++;

		return "added client correctly";
	}

	if (Utility::containsWord(buffer, "GET_ID")){
		sendData(to_string(_clientCount));
	}

	return buffer;
}

bool UDPServer::sendData(){
	if (sendto(UDP::_socket, buffer, _recievedLength, 0, (struct sockaddr*) &UDP::si_other, _socketLength) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool UDPServer::sendData(string data){
	if (sendto(UDP::_socket, data.c_str(), strlen(data.c_str()), 0, (struct sockaddr*) &UDP::si_other, _socketLength) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool UDPServer::sendData(int clientIndex){
	if (_clients.empty())
		return false;

	if (clientIndex >= _clients.size())
		return false;

	UDP::si_other.sin_addr.S_un.S_addr = inet_addr(_clients[clientIndex]->addres);
	UDP::si_other.sin_port = _clients[clientIndex]->port;

	_socketLength = sizeof(UDP::si_other);

	if (sendto(UDP::_socket, buffer, _recievedLength, 0, (struct sockaddr*) &UDP::si_other, _socketLength) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		return false;
	}
	return true;
}

void UDPServer::setMaxClientsCount(int maxClientCount){
	_maxClientCount = maxClientCount;
}

int UDPServer::getMaxClientCount() const{
	return _maxClientCount;
}
