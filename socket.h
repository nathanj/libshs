#ifndef __SHS_SOCKET_H__
#define __SHS_SOCKET_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string>
#include <stdexcept>

namespace shs {

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 1024*4;

class Socket
{
public:
	Socket();
	virtual ~Socket();

	// Server initialization
	bool create();
	bool bind ( const int port );
	bool listen() const;
	bool accept ( Socket& ) const;

	// Client initialization
	bool connect ( const std::string host, const int port );

	// Data Transimission
	bool send ( const std::string ) const;
	int recv ( std::string& ) const;

	void set_non_blocking ( const bool );

	bool is_valid() const { return m_sock != -1; }

private:

	int m_sock;
	sockaddr_in m_addr;
};

class SocketException : public std::runtime_error
{
public:
	SocketException(const char *str) : std::runtime_error(str) {}
	SocketException(const std::string &str) : std::runtime_error(str) {}
};

} // namespace shs

#endif /* __SHS_SOCKET_H__ */
