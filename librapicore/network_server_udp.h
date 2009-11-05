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

class NetworkServerUDP {
	public:
		class ReceivedPacket {
			public:
				std::string s;
				sockaddr_in addr;
		};
		NetworkServerUDP( int port ) : PORT( port )
		{
			_sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
			if( _sock < 0 ) {
				perror( "socket" );
				exit( 1 );
			}

			memset( (char *) &_server_addr, 0, sizeof( _server_addr ) );
			_server_addr.sin_family = AF_INET;
			_server_addr.sin_port = htons( PORT );
			_server_addr.sin_addr.s_addr = htonl( INADDR_ANY );

			if( bind( _sock, (struct sockaddr*) &_server_addr, sizeof( _server_addr ) ) < 0 ) {
				perror( "bind" );
				exit( 1 );
			}

			//set nonblocking
			int flags = fcntl( _sock, F_GETFL );
			flags |= O_NONBLOCK;
			fcntl( _sock, F_SETFL, flags );
		}

		void send( const char* s, sockaddr_in* reply_to )
		{
			//TODO this replies to the most recently received UDP message -> must re arrange it so recv returns the message + sockaddr to respond to
			if( sendto( _sock, s, strlen( s ), 0, (sockaddr*) reply_to, sizeof( sockaddr_in ) ) == -1 ) {
				perror( "sendto" );
				exit( 1 );
			}
		}

		bool recv( ReceivedPacket& received_packet )
		{
			static const int BUFFSIZE = 255;
			char buffer[ BUFFSIZE ];
			/* Receive the word back from the server */
			socklen_t clientlen = sizeof( received_packet.addr );
			int received = recvfrom( _sock, buffer, BUFFSIZE, 0, (struct sockaddr *) &(received_packet.addr), &clientlen );

			if( received < 0 )
				return false;

			//fprintf( stderr, "Client connected: %s\n", inet_ntoa( received_packet.addr.sin_addr ) );

			buffer[received] = '\0';        /* Assure null terminated string */
			//fprintf(stdout, buffer);
			//fprintf(stdout, "\n");
			received_packet.s = std::string( buffer );
			return true;
		}

	private:
		struct sockaddr_in _server_addr;
		int _sock, i; //TODO remove i
		const int PORT;

};

