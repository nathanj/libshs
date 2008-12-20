// http-server.h: A forking HTTP server.

#ifndef __SHS_HTTP_SERVER_H__
#define __SHS_HTTP_SERVER_H__

#include <string>
#include <map>

#include "socket.h"

namespace shs {

/// Implements case insensitive less than for strings.
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

/// A map from string to string using case insensitive comparisons.
typedef std::map< std::string, std::string, less_case_insensitive<std::string> > ssmap;

/// Contains HTTP request information.
struct HttpRequest {
	/// The HTTP method (such as "GET" or "POST").
	std::string method;
	/// The request path including the leading "/".
	std::string path;
	/// The request headers.
	ssmap headers;
	/// The request querystring.
	ssmap querystring;
	/// The request parameters.
	ssmap parameters;
};

/// Contains HTTP response information.
struct HttpResponse {
	/// The response status (such as "200 OK").
	std::string status;
	/// The response headers.
	ssmap headers;
	/// The response body.
	std::string body;
};

/// A HTTP server.
///
/// To use, derive from HttpServer and override HandleResponse().
class HttpServer {
public:
	/// The constructor.
	HttpServer();
	/// The destructor.
	virtual ~HttpServer();
	/// Creates a server which listens on the given port.
	///
	/// When a request is received, the HTTP header will be parsed and sent to
	/// the HandleResponse() function. This function never returns.
	///
	/// If the port is in use, a SocketException will be thrown.
	void Serve(int port);
protected:
	/// Handles the HTTP request.
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
