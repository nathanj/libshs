#include "socket.h"

#include <errno.h>
#include <fcntl.h>

#include <cstring>
#include <iostream>
#include <sstream>

namespace shs {

Socket::Socket() :
	m_sock ( -1 )
{
	memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::~Socket()
{
	if ( is_valid() )
		::close ( m_sock );
}

bool Socket::create()
{
	m_sock = socket ( AF_INET, SOCK_STREAM, 0 );

	if ( ! is_valid() )
		throw SocketException("Could not create socket.");

	// TIME_WAIT - argh
	int on = 1;
	if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR,
				( const char* ) &on, sizeof ( on ) ) == -1 )
		throw SocketException("Could not set socket option.");

	return true;
}

bool Socket::bind ( const int port )
{
	if ( ! is_valid() )
	{
		throw SocketException("Could not create socket.");
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons ( port );

	int bind_return = ::bind ( m_sock,
			( struct sockaddr * ) &m_addr,
			sizeof ( m_addr ) );

	if ( bind_return == -1 )
	{
		std::stringstream ss;
		ss << "Could not bind to port " << port << ".";
		throw std::runtime_error(ss.str());
	}

	return true;
}

bool Socket::listen() const
{
	if ( ! is_valid() )
	{
		throw SocketException("Could not create socket.");
	}

	int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

	if ( listen_return == -1 )
	{
		throw SocketException("Could not listen to socket.");
	}

	return true;
}

bool Socket::accept ( Socket& new_socket ) const
{
	int addr_length = sizeof ( m_addr );
	new_socket.m_sock = ::accept ( m_sock,
			( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

	if ( new_socket.m_sock <= 0 )
		throw SocketException("Socket could not be accepted.");
	else
		return true;
}

bool Socket::send ( const std::string s ) const
{
	int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
	if ( status == -1 )
	{
		throw SocketException("Data could not be sent through socket.");
	}
	else
	{
		return true;
	}
}

int Socket::recv ( std::string& s ) const
{
	char buf [ MAXRECV + 1 ];

	s = "";

	memset ( buf, 0, MAXRECV + 1 );

	int status = ::recv ( m_sock, buf, MAXRECV, 0 );

	if ( status == -1 )
	{
		throw SocketException("Could not receive data through socket.");
	}
	else if ( status == 0 )
	{
		return 0;
	}
	else
	{
		s += buf;
		return status;
	}
}

bool Socket::connect ( const std::string host, const int port )
{
	if ( ! is_valid() )
		throw SocketException("Could not create socket.");

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

	if ( errno == EAFNOSUPPORT )
		throw SocketException("Could not convert ip address.");

	status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( status == 0 )
		return true;
	else
		throw SocketException("Could not connect to host.");
}

void Socket::set_non_blocking ( const bool b )
{
	int opts;

	opts = fcntl ( m_sock, F_GETFL );

	if ( opts < 0 )
	{
		return;
	}

	if ( b )
		opts = ( opts | O_NONBLOCK );
	else
		opts = ( opts & ~O_NONBLOCK );

	fcntl ( m_sock, F_SETFL,opts );
}

} // namespace shs
