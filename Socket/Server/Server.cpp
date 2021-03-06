// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE	64
int main()
{

	WSADATA wsd;
	SOCKET sServer;
	SOCKET sClient;
	int retVal;
	char buf[BUF_SIZE];

	//init socket
	using namespace std;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}

	//create SOCKET
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer) 
	{
		cout << "create socket failed!" << endl;
		WSACleanup();
		return -1;
	}

	//bind() socket
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(9990);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	retVal = bind(sServer, (const struct sockaddr *)&addrServer, sizeof(SOCKADDR_IN));
	if (retVal == SOCKET_ERROR)
	{
		cout << "Bind socket failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	//listen socket
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		cout << "listen socket failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	//accept socket
	cout << "server start..." << endl;
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == sClient)
	{
		cout << "accept socket failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}


	//recv() 
	while (true)
	{
		ZeroMemory(buf, BUF_SIZE);
		retVal = recv(sClient, buf, BUFSIZ, 0);
		if (SOCKET_ERROR == retVal)
		{
			cout << "recv socket failed!" << endl;
			closesocket(sServer);
			closesocket(sClient);
			WSACleanup();
			return -1;
		}

		SYSTEMTIME st;
		GetLocalTime(&st);
		//char sDateTime[30];
		cout << st.wHour <<":"<<st.wMinute<<"\t";
		cout << "recv from:" << inet_ntoa(addrClient.sin_addr) << ":" << addrClient.sin_port <<"::"<< buf << endl;

		if (strcmp(buf, "quit") == 0)
		{
			retVal = send(sClient, "quit", strlen("quit"), 0);
			break;
		}

		else
		{
			char msg[BUF_SIZE];
			ZeroMemory(&msg, BUF_SIZE);
			string str("recv msg:");
			str.append(buf);
			strcpy_s(msg,str.c_str());

			retVal = send(sClient, msg, strlen(msg), 0);
			if (SOCKET_ERROR == retVal)
			{
				cout << "send socket failed!" << endl;
				closesocket(sServer);
				closesocket(sClient);
				WSACleanup();
				return -1;
			}
		}
	}


	//close
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();

	system("pause");
	return 0;
}