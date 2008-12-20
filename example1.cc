// example1.cc: A hello world SHS example.

#include <iostream>
#include <string>
#include <sstream>

#include "http-server.h"

// To create a HTTP server, inherit from shs::HttpServer and override the
// HandleResponse method.
class MyHttpServer : public shs::HttpServer {
protected:
	virtual void HandleResponse(shs::HttpRequest &req,
			shs::HttpResponse &resp);
};

void MyHttpServer::HandleResponse(shs::HttpRequest &req,
		shs::HttpResponse &resp)
{
	resp.body = "<h1>Hello world!</h1>";
}

int main(int argc, char **argv)
{
	// Create the server and start listening on port 9999.
	MyHttpServer server;
	server.Serve(9999);

	return 0;
}

