/*
MIT License

Copyright(c) 2018 Gregory Allen Yotz Jr.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _HTTP_SERVER_HPP_
#define _HTTP_SERVER_HPP_

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <thread>
#include <vector>

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

void middleware(std::map<std::string, std::string> request, std::map<std::string, std::string> response)
{
	printf("here");
}

namespace http
{
class server
{

      private:
	int _port;
	int _sock;
	struct sockaddr_in addr_in;
	std::map<std::string, void (*)(std::map<std::string, std::string>, std::map<std::string, std::string>)> funcs;

      public:
	server()
	{
		_port = -1;
		funcs["*\\*"] = &middleware;
	}

	void setPort(int port)
	{
		_port = port;
	}

	void addEndPoint(std::string endpoint, void (*func)(std::map<std::string, std::string>, std::map<std::string, std::string>))
	{
		funcs[endpoint] = func;
	}

	int operator()()
	{
		_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_sock == -1) {
			return 1;
		}

		memset(&addr_in, 0, sizeof(sockaddr_in));

		addr_in.sin_family = AF_INET;
		addr_in.sin_port = htons(_port);
		addr_in.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(_sock, (struct sockaddr *)&addr_in, sizeof(sockaddr_in)) == -1) {
			close(_sock);
		}

		if (listen(_sock, 10) == -1) {
			close(_sock);
		}
		int fd = -1;
		for (;;) {
			fd = accept(_sock, NULL, NULL);
			if (fd == -1) close(_sock);

			if (0 > fd) {
				close(_sock);
			}

			char buff[24];
			memset(&buff, '\0', sizeof(buff));
			std::stringstream ss;
			while (read(fd, buff, sizeof buff) > 0) {
				ss << buff;
			}

			std::cout << ss.str();
			std::stringstream wr;

			wr << "HTTP/1.1 200 OK";
			wr << "X-Powered-By: Express";
			wr << "Content-Type: application/json; charset=utf-8";
			wr << "Content-Length: 17";
			wr << "Date: Thu, 25 Jan 2018 00:46:30 GMT";
			wr << "Connection: close";
			wr << "{\"hello\":\"world\"}";
			std::string wrstr = wr.str();
			for (std::map<std::string, void (*)(std::map<std::string, std::string>, std::map<std::string, std::string>)>::iterator it = funcs.begin(); it != funcs.end(); ++it) {
				// if(it->first)
				// std::map<std::string, std::string> request, response;
				// funcs["*\\*"](request, response);
			}
			write(fd, wrstr.c_str(), wrstr.length());
		}

		if (shutdown(fd, SHUT_RDWR) == -1) {
			close(fd);
			close(_sock);
		}
		return 0;
	}
};
}

#endif