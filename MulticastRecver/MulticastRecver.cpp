#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
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

	sockaddr_in local_addr;
	ip_mreq multicastAddr;
	int len = sizeof(sockaddr);

	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	local_addr.sin_port = htons(port);

	if (bind(sock, (sockaddr*)&local_addr, sizeof(local_addr)))
	{
		printf("bind port:%d error, errno:%d\n", port, WSAGetLastError());
		goto main_end;
	}

	// 加入组播啦
	multicastAddr.imr_interface.S_un.S_addr = INADDR_ANY;
	multicastAddr.imr_multiaddr.S_un.S_addr = inet_addr(pMulticastAddr);
	setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&multicastAddr, sizeof(multicastAddr));

	while (true)
	{
		char buf[100] = { 0 };
		int recvn = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&local_addr, &len);
		if (recvn < 0)
		{
			printf("recv error:%d\n", WSAGetLastError());
			break;
		}
		buf[recvn] = 0;
		printf("%s\n", buf);
	}
	setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&multicastAddr, sizeof(multicastAddr));


main_end:
	closesocket(sock);
	WSACleanup();

	getchar();
	return 0;
}
