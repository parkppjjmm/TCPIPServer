/* Code Explain
This code is a C++ implementation of a simple TCP server using the Winsock API. 
The server listens on a specified port for incoming connections and then exchanges data with the connected client using predefined packet structures.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<windows.h>

#pragma comment (lib, "Ws2_32.lib")
#define PORT 8000
#define PACKET_SIZE 1000
#define SERVER_IP "xxx.xxx.xxx.xxx" // 서버 아이피
#define CAMERA_BUFFER_Value2 100
#define MAX_RETRIES 5
#define RETRY_INTERVAL 1000

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

	SOCKET hServerSocket;
	hServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bind_result = bind(hServerSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	if (bind_result == SOCKET_ERROR) {
		// Handle error
		cout << "Error binding socket" << endl;
		closesocket(hServerSocket);
		return 1;
	}

	int listen_result = listen(hServerSocket, 5);
	if (listen_result == SOCKET_ERROR) {
		// Handle error
		cout << "Error listening on socket" << endl;
		closesocket(hServerSocket);
		return 1;
	}

	while (true) {
		SOCKET hClientSocket;
		SOCKADDR_IN clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		hClientSocket = accept(hServerSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);

		if (hClientSocket == INVALID_SOCKET) {
			// Handle error
			cout << "Error accepting connection" << endl;
			closesocket(hServerSocket);
			return 1;
		}

		int recv_result = recv(hClientSocket, (char*)&ServerRequest, PACKET_SIZE, 0);
		if (recv_result == SOCKET_ERROR) {
			// Handle error
			cout << "Error receiving data from client" << endl;
			closesocket(hClientSocket);
			continue;
		}

		printf("----------------------------------------------------------------\n");
		printf("-------------------------- Server Part -------------------------\n");
		printf("----------------------------------------------------------------\n");
		printf("Server_RecvData Counter : %d\n", ServerRequest.Counter);
		printf("Server_RecvData Value5 : %d\n", ServerRequest.Value5);
		printf("Server_RecvData Value6 : %d\n", ServerRequest.Value6);
		printf("Server_RecvData Value7 : %d\n", ServerRequest.Value7);
		printf("Server_RecvData Value8 : %d\n", ServerRequest.Value8);

		clientRecv.Counter = clientRecv.Counter + 1;
		strcpy(clientRecv.Value1, "TEST GOOD");
		clientRecv.Value2 = 123456;
		clientRecv.Value3 = 654321;
		clientRecv.Value4 = 1;

		int send_result = send(hClientSocket, (char*)&clientRecv, sizeof(struct _EXAMPLE_SEND_PACKET), 0);
		if (send_result == SOCKET_ERROR) {
			// Handle error
			cout << "Error sending data to client" << endl;
			closesocket(hClientSocket);
			continue;
		}

		closesocket(hClientSocket);
	}

	closesocket(hServerSocket);
	WSACleanup();

	return 0;
}

