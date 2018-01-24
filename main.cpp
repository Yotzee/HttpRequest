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

#include "HttpRequest.hpp"
#include <cstdio>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		std::cout << "<<<<<<<< ERROR >>>>>>>>>" << std::endl;
		std::cout << "Arg 1: host + url" << std::endl;
		std::cout << "Arg 2: port" << std::endl;
		return 1;
	}
	HttpRequest request;
	request.setHost(argv[1]);
	request.setPath(argv[2]);
	request.setPort(atoi(argv[3]));
	request.setHeader("Content-Type", "application/json");
	request.setMethod(HTTP_REQUEST_POST);
	request.setBody("{test:\"hello world\"}");
	request.commit();
	std::cout << request.getBody();
	return 0;
}