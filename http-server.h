#ifndef __SHS_HTTP_SERVER_H__
#define __SHS_HTTP_SERVER_H__

#include <string>
#include <map>

#include "socket.h"

namespace shs {

typedef std::map<std::string, std::string> ssmap;

struct HttpRequest {
	std::string method;
	std::string path;
	ssmap headers;
	ssmap querystring;
	ssmap parameters;
};

struct HttpResponse {
	std::string status;
	ssmap headers;
	std::string body;
};

class HttpServer {
public:
	HttpServer();
	virtual ~HttpServer();
	void Serve(int port);
protected:
	virtual void HandleResponse(HttpRequest &req, HttpResponse &resp);
private:
	void FillHttpRequest(Socket &client_sock, HttpRequest &req);
	void SendResponse(Socket &sock, const HttpResponse &resp);

	// Disallow copying
	HttpServer(const HttpServer&);
	void operator=(const HttpServer&);
};

} // namespace shs

#endif /* __SHS_HTTP_SERVER_H__ */
