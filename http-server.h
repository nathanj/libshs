#ifndef __SHS_HTTP_SERVER_H__
#define __SHS_HTTP_SERVER_H__

#include <string>
#include <map>

#include "socket.h"

namespace shs {

template<class T>
struct less_case_insensitive : std::binary_function<T,T,bool>
{
	inline bool operator()(const T& lhs, const T& rhs)
	{
		typename T::const_iterator i = lhs.begin();
		typename T::const_iterator j = rhs.begin();

		while (i != lhs.end() && j != rhs.end())
		{
			if (tolower(*i) < tolower(*j))
				return true;
			else if (tolower(*i) > tolower(*j))
				return false;

			i++;
			j++;
		}

		return (j != rhs.end());
	}
};

typedef std::map< std::string, std::string, less_case_insensitive<std::string> > ssmap;

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
