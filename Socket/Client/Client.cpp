// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include<WinSock2.h>
#include <string>
#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE	64

int main()
{
	WSADATA wsd;
	SOCKET sHost;
	sockaddr_in sAddr;
	char buf[BUF_SIZE];
	int ret;

	//init socket
	using namespace std;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}

	//socket
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "create socket failed!" << endl;
		WSACleanup();
		return -1;
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sAddr.sin_port = htons(9990);
	int sAdderlen = sizeof(sAddr);

	ret = connect(sHost, (const sockaddr *)&sAddr, sizeof(sAddr));
	if (ret == SOCKET_ERROR)
	{
		cout << "connect socket failed!" << endl;
		closesocket(sHost);
		WSACleanup();
		return -1;
	}

	while (true)
	{
		cout << "please input a string to send: ";
		string str;
		getline(cin, str);

		ZeroMemory(buf, BUF_SIZE);
		strcpy_s(buf, str.c_str());

		ret = send(sHost, buf, strlen(buf), 0);

		if (ret == SOCKET_ERROR)
		{
			cout << "connect socket failed!" << endl;
			closesocket(sHost);
			WSACleanup();
			return -1;
		}

		ZeroMemory(buf, BUF_SIZE);
		ret = recv(sHost, buf, sizeof(buf) + 1, 0);
		cout << "recv buf:" << buf << endl;

		if (strcmp(buf, "quit") == 0)
		{
			cout << "quit!";
			break;
		}
	}

	closesocket(sHost);
	WSACleanup();
}

