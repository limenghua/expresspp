#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include "HttpServer.h"
#include "Connection.h"
#include <iostream>

using namespace expresspp;
using 	boost::asio::ip::tcp;
using	boost::asio::io_service;

HttpServer::HttpServer(RequestHandler handler):
	_requestHandler(handler)
{
	auto acceptHanlder = [this](TcpServer::socket & socket){
		auto conn = Connection::Create(socket.get_io_service(), _requestHandler);
		conn->Socket() = std::move(socket);
		conn->Start();
	};
	_tcpServer = TcpServer::Create(TcpServer::AcceptHander(acceptHanlder));
}

HttpServer::Pointer HttpServer::Listen(unsigned short port)
{
	_tcpServer->Listen(port);
	return shared_from_this();
}

void HttpServer::Run()
{
	_tcpServer->Run();
}