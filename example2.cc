// example2.cc: A SHS example with form posting.

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

#include "http-server.h"

// Here I am hardcoding the favicon, stylesheet, and javascript into the
// program. Of course, these files can be read from the hard drive every
// request instead.

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

const char javascript[] =
"function ajax(url, vars, callbackFunction) {"
"  var request =  new XMLHttpRequest();"
"  request.open('GET', url, true);"
" "
"  request.onreadystatechange = function() {"
"    var done = 4, ok = 200;"
"    if (request.readyState == done && request.status == ok) {"
"      if (request.responseText) {"
"        callbackFunction(request.responseText);"
"      }"
"    }"
"  };"
"  request.send(vars);"
"}"
"function get_time() {"
"  document.getElementById('time').innerHTML = 'loading...';"
"  ajax('/ajaxcall', '', function(response) {"
"    document.getElementById('time').innerHTML = response;"
"  });"
"  return false;"
"}";

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
			"<head>"
			"<link rel='stylesheet' href='/style.css' />"
			"<script language='javascript' src='/script.js'></script>"
			"</head>"
			"<body>"
			"<h1>SHS Test Application</h1>";

		// Display the request headers
		ss << "<h2>Request headers:</h2>";
		ss << "<pre>";
		for (shs::ssmap::const_iterator it = req.headers.begin();
				it != req.headers.end(); ++it)
		{
			ss << "<b>" << it->first << "</b>: " << it->second << std::endl;
		}
		ss << "</pre>";

		// Display the querystring
		ss << "<h2>GET parameters (if any):</h2>";
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
			ss << "<h2>POST parameters (if any):</h2>";
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
			"<h2>Example POST</h2>"
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
			"<p><input type='submit' value='Submit' /></p>"
			"</form>";

		// Display the ajax test
		ss <<
			"<h2>Ajax Example</h2>"
			"<p>"
			"<input type='button' value='Get Time' onclick='get_time()' />"
			"</p>"
			"<p><span id='time'></span></p>";

		ss << "</body></html>";
	}
	else if (req.path == "/style.css")
	{
		// Tell the browser to cache the stylesheet for 10 minutes (600
		// seconds)
		resp.headers["Cache-Control"] = "max-age=600";
		resp.headers["Content-Type"] = "text/css";
		ss << css;
	}
	else if (req.path == "/script.js")
	{
		resp.headers["Content-Type"] = "text/javascript";
		ss << javascript;
	}
	else if (req.path == "/favicon.ico")
	{
		resp.headers["Content-Type"] = "image/x-icon";
		ss << std::string(favicon_ico, favicon_ico+favicon_ico_len);
	}
	else if (req.path == "/ajaxcall")
	{
		resp.headers["Content-Type"] = "text/plain";
		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		ss << asctime(tm);
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

