// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int tcpClient() {
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(6666);
	// serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "192.168.1.4", (void*)&serAddr.sin_addr.S_un.S_addr);
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			printf("connect error !");
			closesocket(sclient);
			return 0;
			//continue;
		}
	while (1) {
		printf("input>>");
		char sendStr[255] = {'\0'};
		scanf_s("%s", sendStr,255);
		//char const * sendData = "你好，TCP服务端，我是客户端!\n";
		//send(sclient, sendData, strlen(sendData), 0);
		send(sclient, sendStr, strlen(sendStr), 0);
		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
			recData[ret] = 0x00;
			printf(recData);
			closesocket(sclient);
			sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
			{
				printf("connect error !");
				closesocket(sclient);
				// return 0;
				continue;
			}
		}
		//closesocket(sclient);
	}
	closesocket(sclient);
	WSACleanup();
	return 0;
}

int tcpServer() {
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
	//绑定IP和端口
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(6666);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
	//循环接收数据
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	printf("等待连接...\n");
	while (true)
	{
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			return 0;
		}
		char sendBuf[20] = { '\0' };
		inet_ntop(AF_INET, (void*)&remoteAddr.sin_addr, sendBuf, 16);
		// printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		printf("接受到一个连接：%s \r\n", sendBuf);
		//接收数据
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf("%s\n",revData);
		}

		//发送数据
		char const * sendData = "你好，TCP客户端！\n";
		printf("send 1\n");
		send(sClient, sendData, strlen(sendData), 0);
		printf("send 2\n");
		closesocket(sClient);
	}
	closesocket(slisten);
	WSACleanup();
	return 0;
}
int main(int argc, char* argv[])
{
	/*std::cout << "argc:" << argc<<std::endl;
	for (int i = 0; i < argc; i++) {
		std::cout << "argc[" << i << "]:" << argv[i] << std::endl;
	}
	if (argc > 1) {
		if (argv[1][0] == 'c') {
			printf("start client...\n");
			tcpClient();
		}
		else if (argv[1][0] == 's') {
			printf("start server...\n");
			tcpServer();
		}
	}*/
	http_server();
	return 0;
}

