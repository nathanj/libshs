#include <iostream>
#include <string>
#include <sstream>

#include "http-server.h"

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
	MyHttpServer server;
	server.Serve(9999);

	return 0;
}

