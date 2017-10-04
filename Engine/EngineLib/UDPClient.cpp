#include "UDPClient.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

struct sockaddr_in si_other;

UDPClient::UDPClient()
{
	_socketLength = sizeof(si_other);
	_buffer = new char[BUFLEN];
}


UDPClient::~UDPClient(){
	delete[] _buffer;

	closesocket(_socket);
	WSACleanup();
}

bool UDPClient::init(){
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return false;
	}
	printf("Initialised.\n");

	//create socket
	if ((_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		return false;
	}
	printf("socket Initialised.\n");

	u_long mode = 1;
	ioctlsocket(_socket, FIONBIO, &mode);

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	return true;
}

bool UDPClient::startListeningData(){
	memset(_buffer, '\0', BUFLEN);
	
	if (recvfrom(_socket, _buffer, BUFLEN, 0, (struct sockaddr *) &si_other, &_socketLength) == SOCKET_ERROR)
	{
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		return false;
	}

	printf("Data: %s\n", _buffer);
	return true;
}

bool UDPClient::sendData(string data){
	if (sendto(_socket, data.c_str(), strlen(data.c_str()), 0, (struct sockaddr *) &si_other, _socketLength) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		return false;
	}

	return true;
}

bool UDPClient::registerToServer(){
	return sendData("REGISTER");
}