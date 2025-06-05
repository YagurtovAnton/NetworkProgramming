#define _CRT_SECURE_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include<Windows.h>
#include<iphlpapi.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iostream>
using namespace std;

#pragma comment(lib, "WS2_32.lib")

#define DEFAULT_PORT			"27015"
#define DEFAULT_BUFFER_LENGTH	1500

void main()
{
	setlocale(LC_ALL, "");
	WSAData wsaData;

	INT iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	//1
	cout << "выћучить теории" << endl;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//2
	addrinfo* result=NULL;// выполн€ем разрешение имен
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		WSACleanup();
		cout << "Error: getaddrinfo failed: " << iResult << endl;
		return;
	}
	//3 создаем сокет дл€ подключение к серверу
	SOCKET connect_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (connect_socket == INVALID_SOCKET)
	{
		cout << "Socket creation failed with code: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	//4 ѕодключаемс€ к серверу
	iResult = connect(connect_socket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Unable to connect to Server:" << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	//5 получение и отправка данных
	CHAR send_buffer[DEFAULT_BUFFER_LENGTH] = "Hello Server, i am Client";
	CHAR recvbuffer[DEFAULT_BUFFER_LENGTH]{};

	do
	{
	iResult = send(connect_socket, send_buffer, strlen(send_buffer),0);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Unable to connect to Server:" << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	//iResult = shutdown(connect_socket, SD_SEND);
	/*if (iResult == SOCKET_ERROR)
	{
		cout << "Shutdown Failed <<" << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}*/
	// 6 Receive data
		iResult = recv(connect_socket, recvbuffer, DEFAULT_BUFFER_LENGTH, 0);
		if (iResult > 0)cout << "ByRec" << iResult << endl;
		else if (result == 0)cout << "Connection closed" << endl;
		else cout << "Receive failed with code: " << WSAGetLastError()<< endl;
		ZeroMemory(send_buffer, sizeof(send_buffer));
		ZeroMemory(recvbuffer, sizeof(recvbuffer));
		cout << "¬ведите сообщение: "; cin.getline(send_buffer, DEFAULT_BUFFER_LENGTH);
	} while (iResult > 0);

	//7) Disconnect:
	iResult = shutdown(connect_socket, SD_SEND);
	closesocket(connect_socket);
	freeaddrinfo(result);
	WSACleanup();
	return;

	system("PAUSE");
} 