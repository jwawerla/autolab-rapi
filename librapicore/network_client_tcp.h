#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <string.h>
#include <fcntl.h>
#include <iostream> //TODO remove this


class NetworkClientTCP {
	public:
		NetworkClientTCP( const std::string& SERVER_IP, int port = 9930 ) : DST_IP( SERVER_IP ), PORT( port )
		{
			_sock = socket( AF_INET, SOCK_STREAM, 0 );
			if( _sock < 0 ) {
				perror( "socket" );
				exit( 1 );
			}

			memset( (char *) &_dstaddr, 0, sizeof( _dstaddr ) );
			_dstaddr.sin_family = AF_INET;
			_dstaddr.sin_port = htons( PORT );
			if ( inet_aton( DST_IP.c_str(), &_dstaddr.sin_addr ) == 0 ) {
				fprintf(stderr, "inet_aton() failed\n");
				exit(1);
			}

			if ( connect( _sock, (struct sockaddr*) &_dstaddr, sizeof( _dstaddr ) ) < 0 ) {
				perror("ERROR connecting");
				exit( 1 );
			}

			//set nonblocking
			int flags = fcntl( _sock, F_GETFL );
			flags |= O_NONBLOCK;
			fcntl( _sock, F_SETFL, flags );
		}

		void send( const std::string& s ) { return send( s.c_str() ); }
		void send( const char* s )
		{
			int n = write( _sock, s, strlen(s) );
			if( n <= 0 ) {
				perror( "write" );
				//exit( 1 );
			}
		}

		bool recv( std::string& buf )
		{
			static const int BUFFSIZE = 4000;
			static char buffer[ BUFFSIZE ];
			/* Receive the word back from the server */

			int n = read( _sock, buffer, BUFFSIZE-1 );
			if( n <= 0 )
				return false;

			buffer[ n ] = '\0';
			buf = std::string( buffer );
			return true;

		}

	private:
		struct sockaddr_in _dstaddr;
		struct sockaddr_in _recv_from_addr;
		int _sock, i;
		const int PORT;
		const std::string DST_IP;

};
