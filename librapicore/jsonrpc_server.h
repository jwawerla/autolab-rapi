#pragma once

#include <map>
#include "tinyjson.hpp"
#include "network_server_udp.h"
#include "network_client_udp.h"
#include "network_server_tcp.h"
#include "network_client_tcp.h"

namespace jsonrpc {

	typedef json::grammar<char>::variant variant;
	typedef json::grammar<char>::object object;
	typedef json::grammar<char>::array array;

	///helper function to wrap anything into a tinyjson variant type
	//TODO can I correctly convert char* to std::string somehow?
	template< typename ValueType > static variant toVariant( const ValueType& value ) { return variant( new boost::any( value ) ); }

	///helper function to quickly convert variant to value - and throws bad_cast if anything fails
	template< typename ValueType > static ValueType fromVariant( const variant& v )
	{ 
		//cant be NULL (in C++ sense)
		if( v.get() == NULL )
			throw std::bad_cast();

		//null json objects can't be cast into anything
		if( v->empty() == true )
			throw std::bad_cast();

		//ensure asked for type is correct
		if( v->type() != typeid( ValueType ) )
			throw std::bad_cast();

		return boost::any_cast< ValueType >( *v );
	}


class Server
{
	private:
		//abstract class for callbacks - its possible to inherit this, but then the callback class can only handle one callback
		//its best to create a RPCMethod instance instead (thats why this is private) 
		class RPCMethodAbstractBase {
			public:
				virtual void call( variant args, object& responce, const std::string& ip, int port ) = 0;
		};

		std::map< std::string, RPCMethodAbstractBase* > _map;

		//function taken from jsoncpp (public domain license)
		static std::string valueToQuotedString( const char *value )
		{
			// Not sure how to handle unicode...
			if (strpbrk(value, "\"\\\b\f\n\r\t") == NULL)
				return std::string("\"") + value + "\"";
			// We have to walk value and escape any special characters.
			// Appending to std::string is not efficient, but this should be rare.
			// (Note: forward slashes are *not* rare, but I am not escaping them.)
			unsigned maxsize = strlen(value)*2 + 3; // allescaped+quotes+NULL
			std::string result;
			result.reserve(maxsize); // to avoid lots of mallocs
			result += "\"";
			for (const char* c=value; *c != 0; ++c){
				switch(*c){
					case '\"':
						result += "\\\"";
					break;
					case '\\':
					result += "\\\\";
					break;
					case '\b':
					result += "\\b";
					break;
					case '\f':
					result += "\\f";
					break;
					case '\n':
					result += "\\n";
					break;
					case '\r':
					result += "\\r";
					break;
					case '\t':
					result += "\\t";
					break;
					case '/':
					// Even though \/ is considered a legal escape in JSON, a bare
					// slash is also legal, so I see no reason to escape it.
					// (I hope I am not misunderstanding something.)
					default:
					result += *c;
				}
			}
			result += "\"";
			return result;
		}

	public:
		static std::string toString( const variant& v )
		{
			if( v->empty() == true ) {
				return "null";
			}
			else if( v->type() == typeid( bool ) )
			{
				// variant is of type "bool"...
				bool b = boost::any_cast< bool >(*v);
				return ( b ? "true" : "false" );
			}
			else if( v->type() == typeid( int ) )
			{
				// variant is of type "int"...
				int i = boost::any_cast< int >(*v);
				std::ostringstream oss;
				oss << i;
				return oss.str();
			}
			else if( v->type() == typeid(double))
			{
				// variant is of type "double"...
				double d = boost::any_cast< double >(*v);
				std::ostringstream oss;
				oss << d;
				//ensure doubles have a decimal
				if( oss.str().find( "." ) == std::string::npos )
					oss << ".0";
				return oss.str();
			}
			else if( v->type() == typeid(std::string))
			{
				// variant is a string...
				std::string s = boost::any_cast< std::string >(*v);
				return valueToQuotedString( s.c_str() );
			}
			else if( v->type() == typeid(json::grammar<char>::array))
			{
				// variant is an array => use recursion
				json::grammar<char>::array const & a = boost::any_cast< json::grammar<char>::array >(*v);

				std::ostringstream oss;
				oss << "[";
				for(json::grammar<char>::array::const_iterator it = a.begin(); it != a.end(); ++it)
				{
					if( it != a.begin() )
						oss << ",";
					oss << toString( *it );
				}
				oss << "]";
				return oss.str();
			}
			else if( v->type() == typeid(json::grammar<char>::object))
			{
				// variant is an object => use recursion
				json::grammar<char>::object const & o = boost::any_cast< json::grammar<char>::object >(*v);

				std::ostringstream oss;
				oss << "{";
				for(json::grammar<char>::object::const_iterator it = o.begin(); it != o.end(); ++it)
				{
					if( it != o.begin() )
						oss << ",";
					std::string key = (*it).first;
					oss << valueToQuotedString( key.c_str() ) << ":" << toString( it->second );
				}
				oss << "}";
				return oss.str();
			}
			else
			{
				assert( 0 );
				// ERROR: unknown type...
			}
		}

		//wrapper for RPC callback methods
		//usage: RPCMethod< MyClass >( &my_class_instance, &MyClass::foo )
		template< class T > 
		class RPCMethod : public RPCMethodAbstractBase {
			public:
				typedef void (T::*method)(variant, object&, const std::string&, int );
				RPCMethod( T *inst, method m ) : _inst( inst ), _meth( m ) {}
				virtual void call( variant args, object& responce, const std::string& ip, int port ) { (_inst->*_meth)( args, responce, ip, port ); }

			private:
				T *_inst;
				method _meth;
		};

		//associate a callback method `m' with jsonrpc `method_name'
		void addMethodHandler( RPCMethodAbstractBase* m, const std::string& method_name ) {
			if( _map[ method_name ] != NULL )
				delete _map[ method_name ];
			_map[ method_name ] = m;
		}

	protected:
		//given a json_str, parse it and call the appropriate RPC method
		//return value: a valid json string to be returned to the client
		std::string call( const std::string& json_str, const std::string& ip, int port )
		{
			static const std::string parse_error( "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32700, \"message\": \"Parse error\"}, \"id\": null}" );
			static const std::string bad_json( "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32600, \"message\": \"Invalid JSON-RPC.\"}, \"id\": null}" );

			variant v = json::parse( json_str.begin(), json_str.end() );

			//check for valid JSON
			if( v->empty() == true ) {
				//unable to parse
				return parse_error;
			}

			//check for valid jsonRPC object
			if( v->type() != typeid( object) ) {
				return bad_json;
			}

			//convert to object
			object o = boost::any_cast< object >( *v );

			//extract method name
			variant method_v = o[ "method" ];
			if( method_v->empty() == true || method_v->type() != typeid( std::string ) ) {
				return bad_json;
			}
			std::string method = boost::any_cast< std::string >( *method_v );

			//extract id
			variant id_v = o[ "id" ];
			if( id_v.get() == NULL )
				id_v = variant( new boost::any() );

			RPCMethodAbstractBase *m = _map[ method ];
			if( m == NULL ) {
				//no RPC found
				std::string error( "{\"jsonrpc\": \"2.0\", \"error\": {\"code\": -32601, \"message\": \"Procedure not found.\"}, \"id\": " );
			}
			assert( m != NULL );
			object result;
			//make call
			try {
				m->call( o[ "params" ], result, ip, port );
			} catch( const std::exception& e ) {
				std::cerr << "caught exception while dispatching RPC: " << e.what() << std::endl;
				//build error response
				object response;
				response[ "id" ] = id_v; //could be null or any variant - we never test the actual type
				response[ "jsonrpc" ] = toVariant( std::string( "2.0" ) );
				object error;
				error[ "code" ] = toVariant( -32603 );
				error[ "message" ] = toVariant( std::string( "Internal error." ) );
				error[ "data" ] = toVariant( e.what() );
				response[ "error" ] = toVariant( error );
				return toString( toVariant( response ) );
			}

			//build response
			object response;
			response[ "id" ] = id_v; //could be null or any variant - we never test the actual type
			response[ "jsonrpc" ] = toVariant( std::string( "2.0" ) );
			response[ "result" ] = toVariant( result );

			std::string return_str( toString( toVariant( response ) ) );
			//std::cout << return_str << std::endl;
			return return_str;

		}
};

class UDPServer : public Server
{
	private:
		NetworkServerUDP _server;
		NetworkServerUDP::ReceivedPacket _received_packet;
	public:
		UDPServer( int port ) : _server( port ) { }
		bool recv( void )
		{
				bool received = _server.recv( _received_packet );
				if( received == true ) {
					std::string ip = inet_ntoa( _received_packet.addr.sin_addr );
					int port = ntohs( _received_packet.addr.sin_port );
					std::string responce = call( _received_packet.s, ip, port );
					_server.send( responce.c_str(), &(_received_packet.addr) );
				}
				return received;
		}
};

class TCPServer : public Server
{
	private:
		NetworkServerTCP _server;
		class DisconnectMethodAbstractBase {
			public:
				virtual void call( const std::string& ip, int port ) = 0;
		};
		DisconnectMethodAbstractBase* _disconnect_method;
	public:

		template< class T > 
		class DisconnectMethod : public DisconnectMethodAbstractBase {
			public:
				typedef void (T::*method)(const std::string&, int );
				DisconnectMethod( T *inst, method m ) : _inst( inst ), _meth( m ) {}
				virtual void call( const std::string& ip, int port ) { (_inst->*_meth)( ip, port ); }

			private:
				T *_inst;
				method _meth;
		};

		void setDisconnectMethod( DisconnectMethodAbstractBase* meth )
		{
			if( _disconnect_method != NULL )
				delete _disconnect_method;
			_disconnect_method = meth;
		}

		TCPServer( int port ) : _server( port ), _disconnect_method( NULL ) { }
		bool recv( void )
		{
			int id;
			std::string buf;
			std::string ip;
			int port;
			bool received = _server.recv( id, buf, ip, port );
			if( received == true ) {
				if( id < 0 ) {
					//disconnect
					if( _disconnect_method != NULL )
						_disconnect_method->call( ip, port );
					return true;
				}
				std::string responce = call( buf, ip, port );
				_server.send( id, responce );
			}
			return received;
		}
};

class UDPClient 
{
	private:
		NetworkClientUDP _client;
		NetworkServerUDP::ReceivedPacket _received_packet;
	public:
		UDPClient( std::string server_ip, int port ) : _client( server_ip, port ) { }

		void call( object o ) { call( toVariant( o ) ); }

		void call( variant v )
		{
			_client.send( Server::toString( v ) );
		}

		bool recv( variant& var )
		{
			bool received = _client.recv( _received_packet );
			if( received == false )
				return false;

			var = json::parse( _received_packet.s.begin(), _received_packet.s.end() );
			return true;
		}
};

class TCPClient 
{
	private:
		NetworkClientTCP _client;
	public:
		TCPClient( std::string server_ip, int port ) : _client( server_ip, port ) { }

		void call( object o ) { call( toVariant( o ) ); }

		void call( variant v )
		{
			_client.send( Server::toString( v ) );
		}

		bool recv( variant& var )
		{
			std::string buf;
			bool received = _client.recv( buf );
			if( received == false )
				return false;

			var = json::parse( buf.begin(), buf.end() );
			return true;
		}
};


}; //end of jsonrpc namespace
