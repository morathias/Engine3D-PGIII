#include "UDPServer.h"

#include <winsock2.h>
#include<stdio.h>

#include "Utility.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

namespace UDP{
	unsigned int _socket;
	struct sockaddr_in _server, si_other;
}

UDPServer::UDPServer(){
	buffer = new char[BUFLEN];
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

	return true;
}

bool UDPServer::startListeningData(){
	//printf("Waiting for data...");
	//fflush(stdout);

	//clear the buffer by filling null, it might have previously received data
	memset(buffer, '\0', BUFLEN);

	//try to receive some data, this is a blocking call
	if ((_recievedLength = recvfrom(UDP::_socket, buffer, BUFLEN, 0, (struct sockaddr *) &UDP::si_other, &_socketLength)) == SOCKET_ERROR)
	{
		//printf("recvfrom() %i failed with error code : %d", _recievedLength, WSAGetLastError());
		return false;
	}

	//print details of the client/peer and the data received
	printf("Received packet from %s:%d\n", inet_ntoa(UDP::si_other.sin_addr), ntohs(UDP::si_other.sin_port));
	printf("Data: %s\n", buffer);

	if (Utility::containsWord(buffer, "REGISTER")){}
		//_clients.push_back(ntohs(UDP::si_other.sin_port));

	
	return true;
}

bool UDPServer::sendData(){
	if (sendto(UDP::_socket, buffer, _recievedLength, 0, (struct sockaddr*) &UDP::si_other, _socketLength) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		return false;
	}
	return true;
}

bool UDPServer::sendData(int clientIndex){
	if (sendto(UDP::_socket, buffer, _recievedLength, 0, (struct sockaddr*) &UDP::si_other, _socketLength) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		return false;
	}
	return true;
}

void UDPServer::setMaxClientsCount(int maxClientCount){

}

int UDPServer::getMaxClientCount() const{
	return _maxClientCount;
}
