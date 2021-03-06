#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

const char *pMulticastAddr = "234.0.0.1";
const u_short port = 8888;

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	int n = 0;
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&n, sizeof(n));

	sockaddr_in addr;
	addr.sin_addr.S_un.S_addr = inet_addr(pMulticastAddr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	static int i = 0;
	while (1)
	{
		char buf[100] = { 0 };
		sprintf(buf, "blablablabla:%d", i++);
		int sendn = sendto(sock, buf, strlen(buf) + 1, 0, (sockaddr*)&addr, sizeof(sockaddr));
		printf("send:[%s] %dB\n", buf, sendn);
		Sleep(500);
	}

	closesocket(sock);
	WSACleanup();

	return 0;
}
