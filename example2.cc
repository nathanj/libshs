#include <iostream>
#include <string>
#include <sstream>

#include "http-server.h"

// Here I am hardcoding the favicon and stylesheet into the program. Of course,
// these files can be read from the hard drive every request instead.

// Generated with 'xxd -i favicon.ico'
const unsigned char favicon_ico[] = {
  0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x02, 0x00, 0x01, 0x00,
  0x01, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
  0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x1b, 0x87, 0x00, 0x00, 0xdb, 0xb7,
  0x00, 0x00, 0xdb, 0xb7, 0x00, 0x00, 0xdb, 0xb7, 0x00, 0x00, 0xdb, 0xb7,
  0x00, 0x00, 0xdb, 0xb7, 0x00, 0x00, 0xd8, 0x37, 0x00, 0x00, 0xdb, 0xb7,
  0x00, 0x00, 0xdb, 0xb7, 0x00, 0x00, 0xdb, 0xb7, 0x00, 0x00, 0xdb, 0xb7,
  0x00, 0x00, 0xc3, 0xb1, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned int favicon_ico_len = 198;

const char css[] =
"body { font-family: verdana, sans-serif; padding: 10px; }"
"h1   { border-bottom: 1px solid #ccc; }"
"pre  { background-color: #eee }";

class MyHttpServer : public shs::HttpServer {
protected:
	virtual void HandleResponse(shs::HttpRequest &req,
			shs::HttpResponse &resp);
};

void MyHttpServer::HandleResponse(shs::HttpRequest &req,
		shs::HttpResponse &resp)
{
	std::stringstream ss;

	if (req.path == "/")
	{
		// Display the header
		ss <<
			"<html>"
			"<head><link rel='stylesheet' href='/style.css' /></head>"
			"<body>"
			"<h1>SHS Test Application</h1>";

		// Display the request headers
		ss << "<p>Request headers:</p>";
		ss << "<pre>";
		for (shs::ssmap::const_iterator it = req.headers.begin();
				it != req.headers.end(); ++it)
		{
			ss << "<b>" << it->first << "</b>: " << it->second << std::endl;
		}
		ss << "</pre>";

		// Display the querystring
		ss << "<p>GET parameters (if any):</p>";
		ss << "<ul>";
		for (shs::ssmap::const_iterator it = req.querystring.begin();
				it != req.querystring.end(); ++it)
		{
			ss << "<li>" << it->first << " = " << it->second << "</li>";
		}
		ss << "</ul>";

		// Display the post parameters
		if (req.method == "POST")
		{
			ss << "<p>POST parameters (if any):</p>";
			ss << "<ul>";
			for (shs::ssmap::const_iterator it = req.parameters.begin();
					it != req.parameters.end(); ++it)
			{
				ss << "<li>" << it->first << " = " << it->second << "</li>";
			}
			ss << "</ul>";
		}

		// Display the form
		ss <<
			"<form method='POST' action='/?submit' name='nameform'>"
			"<p>Enter your name:"
			"<input type='textbox' name='name' value='"
			<< req.parameters["name"] <<
			"' /></p>"
			"<p>Choose a language:"
			"<select name='language'>"
			"<option value='c'>C</option>"
			"<option value='c++'>C++</option>"
			"<option value='python'>Python</option>"
			"</select></p>"
			"<p><input type='submit' value='Submit'></p>"
			"</form>";

		ss << "</body></html>";
	}
	else if (req.path == "/style.css")
	{
		resp.headers["Content-Type"] = "text/css";
		ss << css;
	}
	else if (req.path == "/favicon.ico")
	{
		resp.headers["Content-Type"] = "image/x-icon";
		ss << std::string(favicon_ico, favicon_ico+favicon_ico_len);
	}
	else
	{
		resp.status = "404 Not Found";
		ss << "404 Not Found";
	}

	resp.body = ss.str();
}

int main(int argc, char **argv)
{
	MyHttpServer server;
	server.Serve(9999);

	return 0;
}

