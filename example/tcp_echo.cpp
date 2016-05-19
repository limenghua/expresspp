#include <http_server\TcpServer.h>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include <iostream>

using namespace expresspp;
using 	boost::asio::ip::tcp;

void echo(tcp::socket &socket,const char * data,size_t len)
{
	socket.send(boost::asio::buffer(data, len));
}

void TcpEcho()
{
	try{
		auto server = TcpServer::Create(TcpServer::RecvHandler(echo));
		server->Listen(8082);
		server->Run();
	}
	catch (std::exception & e)
	{
		std::cerr << e.what();
	}
}