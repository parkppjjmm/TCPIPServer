#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>

#pragma comment (lib, "Ws2_32.lib")

#define PORT 8000
#define PACKET_SIZE 1000
#define SERVER_IP "xxx.xxx.xxx.xxx" // 서버 아이피
#define CAMERA_BUFFER_Value2 100

using namespace std;

typedef struct _EXAMPLE_SEND_PACKET
{
	int32_t Counter;
	char Value1[16];
	int32_t Value2;
	int32_t Value3;
	int32_t Value4;
}_EXAMPLE_SEND_PACKET;

typedef struct _EXAMPLE_RECV_PACKET {
	int32_t Counter;
	bool Value5;
	bool Value6;
	bool Value7;
	bool Value8;
}_EXAMPLE_RECV_PACKET;

struct _EXAMPLE_SEND_PACKET clientRecv;
struct _EXAMPLE_RECV_PACKET ServerRequest;

int main(int argc, char* argv[], char* envp[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hServerSocket, hClientSocket;
	hServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tServerAddr = {};
	tServerAddr.sin_family = AF_INET;
	tServerAddr.sin_port = htons(PORT);
	tServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hServerSocket, (SOCKADDR*)&tServerAddr, sizeof(tServerAddr));
	listen(hServerSocket, 5);

	SOCKADDR_IN tClientAddr = {};
	int iClientAddrLen = sizeof(tClientAddr);
	hClientSocket = accept(hServerSocket, (SOCKADDR*)&tClientAddr, &iClientAddrLen);

	while (true)
	{
		recv(hClientSocket, (char*)&ServerRequest, sizeof(struct _EXAMPLE_RECV_PACKET), 0);

		clientRecv.Counter = ServerRequest.Counter + 1;
		strncpy(clientRecv.Value1, "TEST GOOD", 16);
		clientRecv.Value2 = 123456;
		clientRecv.Value3 = 654321;
		clientRecv.Value4 = 1;

		send(hClientSocket, (char*)&clientRecv, sizeof(struct _EXAMPLE_SEND_PACKET), 0);

	}

	closesocket(hClientSocket);
	closesocket(hServerSocket);

	WSACleanup();
	return 0;
}