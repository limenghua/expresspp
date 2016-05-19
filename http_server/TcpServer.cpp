#include "TcpServer.h"
#include "TcpConnection.h"
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <iostream>

using namespace expresspp;
using 	boost::asio::ip::tcp;
using	boost::asio::io_service;

TcpServer::TcpServer(RecvHandler handler) :
_recvHandler(handler),
_io_service(),
_acceptor(_io_service)
{

}

TcpServer::TcpServer(AcceptHander handler):
_io_service(),
_acceptor(_io_service),
_acceptHandler(handler)
{
}

TcpServer::Pointer TcpServer::Listen(unsigned short port)
{
	tcp::endpoint endpoint(tcp::v4(), port);
	_acceptor.open(endpoint.protocol());
	_acceptor.set_option(tcp::acceptor::reuse_address(true));
	_acceptor.bind(endpoint);
	_acceptor.listen();

	auto self = shared_from_this();
	boost::asio::spawn(_io_service,
		[this, self](boost::asio::yield_context yield)
	{
		for (;;){
			boost::system::error_code ec;
			socket s(_io_service);
			_acceptor.async_accept(s, yield[ec]);
			if (ec){
				std::cerr << "Accept new connection failed: " << ec.message();
				continue;
			}

			if (_acceptHandler){
				_acceptHandler(s);
			}
			else if (_recvHandler){
				auto connection = TcpConnection::Create(_io_service, _recvHandler);
				connection->Socket() = std::move(s);
				connection->Start();
			}
		}
	});

	std::cout << "Server Listening on:" << port << std::endl;

	return shared_from_this();
}

void TcpServer::Run()
{
	_io_service.run();
}
