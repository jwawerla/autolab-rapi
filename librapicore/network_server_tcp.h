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
#include <map>
#include <string>

class NetworkServerTCP {
	private:
		/* master file descriptor list */
		fd_set master;
		/* temp file descriptor list for select() */
		fd_set read_fds;
		int _fdmax;
		//maps socket to IP/port
		std::map< int, std::string > _ip;
		std::map< int, int > _port;

	public:
		//class ReceivedPacket {
		//	public:
		//		std::string s;
		//		sockaddr_in addr;
		//};
		NetworkServerTCP( int port ) : PORT( port )
		{
			_sock = socket( AF_INET, SOCK_STREAM, 0 );
			if( _sock < 0 ) {
				perror( "socket" );
				exit( 1 );
			}

			int yes = 1;
			if(setsockopt( _sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
			{
				perror("Server-setsockopt() error");
				exit(1);
			}


			memset( (char *) &_server_addr, 0, sizeof( _server_addr ) );
			_server_addr.sin_family = AF_INET;
			_server_addr.sin_port = htons( PORT );
			//_server_addr.sin_addr.s_addr = htonl( INADDR_ANY ); //TODO verify if htonl is required with INADDR_ANY
			_server_addr.sin_addr.s_addr = INADDR_ANY; //TODO verify if htonl is required with INADDR_ANY

			if( bind( _sock, (struct sockaddr*) &_server_addr, sizeof( _server_addr ) ) < 0 ) {
				perror( "bind" );
				exit( 1 );
			}

			//set nonblocking
			int flags = fcntl( _sock, F_GETFL );
			flags |= O_NONBLOCK;
			fcntl( _sock, F_SETFL, flags );

			if( listen( _sock, 10 ) == -1 ) {
				perror( "listen" );
				exit( 1 );
			}
			_fdmax = -1;
			FD_ZERO( &master );
		}

		bool recv( int& id, std::string& rx, std::string& ip, int& port )
		{

			//try to accept new connections
			{
				struct sockaddr_in clientaddr;
				socklen_t addrlen = sizeof( clientaddr );
				int newfd = accept( _sock, (struct sockaddr *)&clientaddr, &addrlen);
				if( newfd >= 0 ) {
					//accept new connection - save in selection list 
					FD_SET( newfd, &master );
					if( newfd > _fdmax ) {
						_fdmax = newfd; //keep track of max
					}
					//printf("New connection from %s on socket %d\n", inet_ntoa(clientaddr.sin_addr), newfd);
					_ip[ newfd ] = std::string( inet_ntoa( clientaddr.sin_addr ) );
					_port[ newfd ] = ntohs( clientaddr.sin_port );
				}
			}

			//select all connections (but not accepting server)
			read_fds = master;
			struct timeval timeout;
			timeout.tv_sec = timeout.tv_usec = 0;
			if( select( _fdmax+1, &read_fds, NULL, NULL, &timeout ) == -1 )
			{
				perror("Server-select() error");
				exit(1);
			}

			for( int i = 0; i <= _fdmax; i++ ) {
				if( !FD_ISSET(i, &read_fds) )
					continue;

				//process existing connections
				char buf[ 5000 ];
				int nbytes = ::recv(i, buf, sizeof(buf), 0);
				if( nbytes <= 0 ) {
					/* got error or connection closed by client */
					if(nbytes == 0) {
						/* connection closed */
						//std::cerr << "socket " << i << " hung up" << std::endl;
						id = -1; //-1 means disconnect
						rx = ""; 
						ip = _ip[ i ];
						port = _port[ i ];
					} else {
						perror("recv() error");
					}

					/* close it... */
					close(i);
					/* remove from master set */
					FD_CLR(i, &master);
					return true;
				}

				//process buffer
				buf[ nbytes ] = '\0';
				id = i;
				rx = std::string( buf );
				ip = _ip[ i ];
				port = _port[ i ];
				//FIXME could starve sockets with larger numbers - since on re-entry, we will start at the lowest socket
				return true; //indicate there's more messages to read
			}
			return false;
		}

		void send( int id, const std::string& str )
		{
			if( ::send( id, str.c_str(), str.length(), 0) == -1 )
				perror("send() error");
		}

	private:
		struct sockaddr_in _server_addr;
		int _sock, i; //TODO remove i
		const int PORT;
};

