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

#include "network_server_udp.h"

class NetworkClientUDP {
	public:
		NetworkClientUDP( const std::string& SERVER_IP, int port = 9930 ) : DST_IP( SERVER_IP ), PORT( port )
		{
			_sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
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

			//set nonblocking
			int flags = fcntl( _sock, F_GETFL );
			flags |= O_NONBLOCK;
			fcntl( _sock, F_SETFL, flags );
		}

		void send( const std::string& s ) { return send( s.c_str() ); }
		void send( const char* s )
		{
			if( sendto( _sock, s, strlen( s ), 0, (sockaddr*) &_dstaddr, sizeof( _dstaddr ) ) == -1 ) {
				perror( "sendto" );
				//exit( 1 );
			}
		}

		bool recv( NetworkServerUDP::ReceivedPacket& received_packet )
		{
			static const int BUFFSIZE = 255;
			char buffer[ BUFFSIZE ];
			/* Receive the word back from the server */
			socklen_t clientlen = sizeof( received_packet.addr );
			int received = recvfrom( _sock, buffer, BUFFSIZE, 0, (struct sockaddr *) &(received_packet.addr), &clientlen );

			if( received < 0 )
				return false;

			/* Check that client and server are using same socket */
			if( _dstaddr.sin_addr.s_addr != received_packet.addr.sin_addr.s_addr ) {
				//TODO FIXME make robust
				std::cerr << "Received a packet from an unexpected server";
				return false;
				exit( 1 );
			}
			buffer[received] = '\0';
			received_packet.s = std::string( buffer );
			return true;
		}

	private:
		struct sockaddr_in _dstaddr;
		struct sockaddr_in _recv_from_addr;
		int _sock, i;
		const int PORT;
		const std::string DST_IP;

};
