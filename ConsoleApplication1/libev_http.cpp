#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include<string>
#include <WS2tcpip.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <assert.h>
#include <strstream>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int init_win_socket()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return -1;
	}
	return 0;
}

string readHtml(string htmlFile) {
	ifstream infile;
	string htmlLine;
	string html;
	infile.open(htmlFile.data());
	//cout << "456" << endl;
	while (getline(infile, htmlLine)) {
		//cout << htmlLine;
		//cout << "123" << endl;
		html += htmlLine;
	}
	return html;
}

void generic_handler(struct evhttp_request *req, void *arg)
{
	struct evbuffer *buf = evbuffer_new();
	if (!buf)
	{
		puts("failed to create response buffer \n");
		return;
	}

	evbuffer_add_printf(buf, "Server Responsed. Requested: %s\n", evhttp_request_get_uri(req));
	//evbuffer_add_printf(buf, "<input>edit something</>");
	string html = readHtml("index.html");
	//string html = readHtml("D:\\vs_source\\ConsoleApplication1\\Debug\\test.txt");
	//string html = readHtml(".\\test.txt");
	//printf("hello\n");
	printf("%s\n", html.c_str());
	evbuffer_add_printf(buf, html.c_str());
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	evbuffer_free(buf);
}

int http_server() {
#ifdef WIN32
	init_win_socket();
#endif

	// short http_port = 6667;
	short http_port = 39999;
	//const char *http_addr = "127.0.0.1";
	const char *http_addr = "0.0.0.0";
	//const char *http_addr = "192.168.0.104";

	struct event_base * base = event_base_new();

	struct evhttp * http_server = evhttp_new(base);
	if (!http_server)
	{
		return -1;
	}

	int ret = evhttp_bind_socket(http_server, http_addr, http_port);
	if (ret != 0)
	{
		return -1;
	}

	evhttp_set_gencb(http_server, generic_handler, NULL);

	printf("http server start OK! \n");

	event_base_dispatch(base);

	evhttp_free(http_server);

	WSACleanup();
	return 0;
}