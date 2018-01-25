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

#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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

namespace http
{

#define HTTP_METHOD_GET "GET"
#define HTTP_METHOD_POST "POST"
#define HTTP_METHOD_DELETE "DELETE"
#define HTTP_METHOD_PUT "PUT"

#define HTTP_ERROR 0
#define HTTP_SUCCESS 1
#define HTTP_HOST_UNKNOWN 2

#define HTTP_HEADER_PATH "PATH_HEADER"
#define HTTP_HEADER_HOST "Host"
#define HTTP_HEADER_METHOD "METHOD_HEADER"
#define HTTP_HEADER_HTML_1_1 "HTTP/1.1"
#define HTTP_HEADER_SPACE " "
#define HTTP_HEADER_EL "\r\n"
#define HTTP_HEADER_UA "User-Agent"
#define HTTP_HEADER_ACCEPT "Accept"
#define HTTP_HEADER_ACCEPT_LANG "Accept-Language"
#define HTTP_HEADER_CHAR_SET "Accept-Charset"
#define HTTP_HEADER_CONNECTION "Connection"
#define HTTP_HEADER_BODY "BODY"
#define HTTP_HEADER_CONTENT_LENGTH "Content-Length"

#define HTTP_USER_AGENT "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.103 Safari/537.36"

#define HTTP_RESPONSE_STATUS "STATUS"
#define HTTP_RESPONSE_BODY "BODY"
class request
{
      private:
	int _port;
	struct sockaddr_in address;
	struct sockaddr_in serv_addr;
	int sock;
	int valread;
	char buffer[1024];
	std::string _response;
	std::map<std::string, std::string> _responseMap;
	std::map<std::string, std::string> _headers;

      public:
	void operator()()
	{
		commit();
	}

	std::string operator()(const char *host, const char *method, const char *path, int port)
	{
		this->setHost(host);
		this->setMethod(method);
		this->setPath(path);
		this->setPort(port);

		this->commit();
		return this->getBody();
	}

	std::string operator()(const char *host, const char *method, const char *path, int port, const char *body)
	{
		this->setHost(host);
		this->setMethod(method);
		this->setPath(path);
		this->setPort(port);
		this->setBody(body);

		this->commit();
		return this->getBody();
	}

	std::string operator()(const char *host, const char *method, const char *path, int port, const char *body, int headerCount, const char *headers[])
	{
		this->setHost(host);
		this->setMethod(method);
		this->setPath(path);
		this->setPort(port);
		this->setBody(body);
		for (int i = 0; i < headerCount; i = i + 2) {
			this->setHeader(headers[i], headers[i + 1]);
		}
		this->commit();
		return this->getBody();
	}

	request()
	{
		sock = 0;
		valread = 0;
		_port = 80;
		setMethod(HTTP_METHOD_GET);
		memset(buffer, '\0', sizeof(buffer));
		memset(&serv_addr, 0, sizeof(sockaddr_in));
		_headers[HTTP_HEADER_UA] = HTTP_USER_AGENT;
		_headers[HTTP_HEADER_ACCEPT] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
		_headers[HTTP_HEADER_ACCEPT_LANG] = "en-us,en;q=0.5";
		_headers[HTTP_HEADER_CHAR_SET] = "ISO-8859-1,utf-8;q=0.7,*;q=0.7";
		_headers[HTTP_HEADER_CONNECTION] = "Close";
	}

	void setMethod(std::string method)
	{
		setMethod(method.c_str());
	}

	void setMethod(const char *method)
	{
		if (strcmp(method, HTTP_METHOD_GET) == 0) _headers[HTTP_HEADER_METHOD] = HTTP_METHOD_GET;
		if (strcmp(method, HTTP_METHOD_POST) == 0) _headers[HTTP_HEADER_METHOD] = HTTP_METHOD_POST;
		if (strcmp(method, HTTP_METHOD_DELETE) == 0) _headers[HTTP_HEADER_METHOD] = HTTP_METHOD_DELETE;
		if (strcmp(method, HTTP_METHOD_PUT) == 0) _headers[HTTP_HEADER_METHOD] = HTTP_METHOD_PUT;
	}

	void setHeader(std::string headerName, std::string value)
	{
		setHeader(headerName.c_str(), value.c_str());
	}

	void setHeader(const char *headerName, const char *value)
	{
		_headers[headerName] = value;
	}

	void setHost(std::string hostName)
	{
		setHost(hostName.c_str());
	}

	void setHost(const char *hostName)
	{
		_headers[HTTP_HEADER_HOST] = std::string(hostName);
	}

	void setPath(std::string path)
	{
		setPath(path.c_str());
	}

	void setPath(const char *path)
	{
		_headers[HTTP_HEADER_PATH] = std::string(path);
	}

	void setPort(int port)
	{
		if (port == 0) _port = 80;
		_port = port;
	}

	void setBody(std::string body)
	{
		setBody(body.c_str());
	}

	void setBody(const char *body)
	{
		_headers[HTTP_HEADER_BODY] = body;
	}

	std::string getResponse()
	{
		return _response;
	}

	std::string getBody()
	{
		return _responseMap[HTTP_RESPONSE_BODY];
	}

	std::string getStatus()
	{
		return _responseMap[HTTP_RESPONSE_STATUS];
	}

      private:
	inline std::string buildHeader()
	{
		std::string header;
		header.append(_headers[HTTP_HEADER_METHOD] + HTTP_HEADER_SPACE);
		header.append(_headers[HTTP_HEADER_PATH] + HTTP_HEADER_SPACE + HTTP_HEADER_HTML_1_1 + HTTP_HEADER_EL);
		if (_headers[HTTP_HEADER_BODY].length() > 0) {
			char length[10];
			sprintf(length, "%d", (int)_headers[HTTP_HEADER_BODY].length());
			header.append(HTTP_HEADER_CONTENT_LENGTH);
			header.append(": ");
			header.append(length);
			header.append(HTTP_HEADER_EL);
		}

		for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
			if ((it->first == HTTP_HEADER_METHOD) || (it->first == HTTP_HEADER_PATH || it->first == HTTP_HEADER_BODY)) continue;

			header.append(it->first + ": " + _headers[it->first] + HTTP_HEADER_EL);
		}
		header.append(HTTP_HEADER_EL);

		if (_headers[HTTP_HEADER_BODY].length() > 0) {
			header.append(_headers[HTTP_HEADER_BODY]);
		}
		return header;
	}

      public:
	int commit()
	{
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(_port);
		struct hostent *host;

		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
			return HTTP_ERROR;
		}
		setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)1, sizeof(int));

		if ((host = gethostbyname(_headers[HTTP_HEADER_HOST].c_str())) == NULL) {
			return HTTP_HOST_UNKNOWN;
		}

		memcpy(&serv_addr.sin_addr, host->h_addr, host->h_length);

		if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
			close(sock);
			return HTTP_HOST_UNKNOWN;
		}

		std::string headers = buildHeader();
		/// Send Headers
		write(sock, headers.c_str(), headers.length());

		// Read Response
		std::stringstream ss;
		while (read(sock, &buffer, sizeof(buffer)) > 0) {
			ss << buffer;
		};

		_response = ss.str();
		if (_response.length() > 0) {
			_responseMap[HTTP_RESPONSE_STATUS] = _response.substr(9, 3);
			const char *ff = _response.c_str();
			for (int i = 0; i <= _response.length(); i++) {
				if (i + 4 > _response.length()) break;
				if ((ff[i] == '\r' && ff[i + 1] == '\n') && (ff[i + 2] == '\r' && ff[i + 3] == '\n')) {
					_responseMap[HTTP_RESPONSE_BODY] = _response.substr(i + 4, _response.length() - i);
				}
			}
		}
		std::cout << _response;
		close(sock);
		return HTTP_SUCCESS;
	}
};
}
#endif
