/*
MIT License

Copyright (c) 2018 Gregory Allen Yotz Jr.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "http/request.hpp"
#include "http/server.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		std::cout << "<<<<<<<< ERROR >>>>>>>>>" << std::endl;
		std::cout << "Arg 1: host + url" << std::endl;
		std::cout << "Arg 2: port" << std::endl;
		return 1;
	}

	// http::server server;
	// server.setPort(3000);
	// server.listen();

	// http::request request;
	// request.setHost(argv[1]);
	// request.setPath(argv[2]);
	// request.setPort(atoi(argv[3]));
	// request.setHeader("Content-Type", "application/json");
	// request.setMethod(HTTP_METHOD_GET);
	// request();
	// std::cout << request.getBody() << std::endl;

	//http::request r1;
	//	std::cout << r1(argv[1], HTTP_METHOD_GET, argv[2], atoi(argv[3])) << std::endl;
	http::request r2;
	const char *headers[4] = {"Content-Type", "application/json"};
	//std::cout << r2(argv[1], HTTP_METHOD_POST, argv[2], atoi(argv[3]), "{test: \"hello world\"}", 2, headers) << std::endl;
	std::cout << r2(argv[1], HTTP_METHOD_POST, argv[2], atoi(argv[3]), "{\"test\": \"hello world\"}", 2, headers) << std::endl;
	return 0;
}
