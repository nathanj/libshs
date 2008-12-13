#include "http-server.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

namespace shs {

template <typename T>
static std::string tostring(T t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

static unsigned char decode(unsigned char a, unsigned char b)
{
	unsigned char p, q;
	p = (a >= '0' && a <= '9') ? a - '0' : a - 'A' + 10;
	q = (b >= '0' && b <= '9') ? b - '0' : b - 'A' + 10;
	return p*16 + q;
}

HttpServer::HttpServer()
{
}

HttpServer::~HttpServer()
{
	
}

void HttpServer::FillHttpRequest(Socket &client_sock, HttpRequest &req)
{
	bool gotten_method = false;
	bool gotten_path = false;
	bool gotten_querystring = false;
	bool gotten_headers = false;
	std::string tmp, header;
	std::string body;

	while (!gotten_headers)
	{
		std::string data;
		//std::cout << "requesting..." << std::endl;
		client_sock.recv(data);
		//std::cout << "Received: " << data.size() << std::endl;
		//std::cout << data << std::endl;

		std::string::const_iterator p = data.begin();

		while (p != data.end() && !gotten_headers)
		{
			if (!gotten_method)
			{
				if (*p == ' ')
				{
					req.method = tmp;
					tmp.clear();
					gotten_method = true;
				}
				else
				{
					tmp.push_back(*p);
				}
			}
			else if (!gotten_path)
			{
				if (*p == ' ')
				{
					req.path = tmp;
					tmp.clear();
					gotten_path = true;
					gotten_querystring = true;

					// Read past the HTTP version
					while (*p != '\n') p++;
				}
				else if (*p == '?')
				{
					req.path = tmp;
					tmp.clear();
					gotten_path = true;
				}
				else
				{
					tmp.push_back(*p);
				}
			}
			else if (!gotten_querystring)
			{
				if (*p == '=')
				{
					header = tmp;
					tmp.clear();
				}
				else if (*p == '&')
				{
					if (header.empty())
						req.querystring[tmp] = "1";
					else
						req.querystring[header] = tmp;
					header.clear();
					tmp.clear();
				}
				else if (*p == '+')
				{
					tmp.push_back(' ');
				}
				else if (*p == '%')
				{
					// need to turn these calls into two passes
					tmp.push_back(decode(*(p+1), *(p+2)));
					p+=2;
				}
				else if (*p == ' ')
				{
					if (header.empty())
						req.querystring[tmp] = "1";
					else
						req.querystring[header] = tmp;
					header.clear();
					tmp.clear();
					gotten_querystring = true;
					// Read past the HTTP version
					while (*p != '\n') p++;
				}
				else
				{
					tmp.push_back(*p);
				}
			}
			else if (!gotten_headers)
			{
				if (*p == ':' && header.empty())
				{
					header = tmp;
					tmp.clear();
				}
				else if (*p == '\r')
				{
					// Ignore carriage returns
				}
				else if (*p == '\n')
				{
					if (header.empty())
					{
						gotten_headers = true;
						// need to fill body now
						tmp.clear();
					}
					else
					{
						req.headers[header] = tmp;
						header.clear();
						tmp.clear();
					}
				}
				else
				{
					tmp.push_back(*p);
				}
			}

			p++;
		}
	}

	if (req.headers.find("Content-Length") != req.headers.end())
	{
		size_t size = atoi(req.headers["Content-Length"].c_str());
		//std::cout << "size = " << size << std::endl;
		while (body.size() < size)
		{
			std::string data;
			//std::cout << "requesting..." << std::endl;
			client_sock.recv(data);
			//std::cout << "Received: " << data.size() << std::endl;
			//std::cout << data << std::endl;

			body += data;
		}

		std::string::const_iterator p = body.begin();
		while (p != body.end())
		{
			if (*p == '=')
			{
				header = tmp;
				tmp.clear();
			}
			else if (*p == '&')
			{
				req.parameters[header] = tmp;
				header.clear();
				tmp.clear();
			}
			else if (*p == '+')
			{
				tmp.push_back(' ');
			}
			else if (*p == '%')
			{
				tmp.push_back(decode(*(p+1), *(p+2)));
				p+=2;
			}
			else
			{
				tmp.push_back(*p);
			}

			p++;
		}

		req.parameters[header] = tmp;
	}
}

void HttpServer::SendResponse(Socket &sock, const HttpResponse &resp)
{
	std::stringstream ss;
	ss << "HTTP/1.0 " << resp.status << "\r\n";
	for (ssmap::const_iterator it = resp.headers.begin();
			it != resp.headers.end(); ++it)
	{
		ss << it->first << ": " << it->second << "\r\n";
	}

	ss << "\r\n";
	ss << resp.body;

	sock.send(ss.str());
}

void HttpServer::Serve(int port)
{
	Socket socket;
	socket.create();
	socket.bind(port);
	socket.listen();

	while ( true )
	{
		Socket client_sock;
		socket.accept(client_sock);

		HttpRequest req;
		HttpResponse resp;
		FillHttpRequest(client_sock, req);

#if 0
		std::cout << "Parsed: " << std::endl;
		std::cout << req.method << std::endl;
		std::cout << req.path << std::endl;

		std::cout << "querystring: " << req.querystring.size() << std::endl;
		for (ssmap::const_iterator it = req.querystring.begin();
				it != req.querystring.end(); ++it)
		{
			std::cout << it->first << ": " << it->second << std::endl;
		}
		for (ssmap::const_iterator it = req.headers.begin();
				it != req.headers.end(); ++it)
		{
			std::cout << it->first << ": " << it->second << std::endl;
		}
		std::cout << "parameters: " << req.parameters.size() << std::endl;
		for (ssmap::const_iterator it = req.parameters.begin();
				it != req.parameters.end(); ++it)
		{
			std::cout << it->first << ": " << it->second << std::endl;
		}
		std::cout << "\n\n" << std::endl;
#endif
		
		// Add some default headers
		resp.status = "200 OK";
		resp.headers["Content-Type"] = "text/html; charset=UTF-8";
		HandleResponse(req, resp);
		resp.headers["Content-Length"] = tostring(resp.body.size());

		SendResponse(client_sock, resp);
	}
}

void HttpServer::HandleResponse(HttpRequest &req, HttpResponse &resp)
{
	resp.status = "404 Not Found";
	resp.body = "404 Not Found";
}

} // namespace shs

