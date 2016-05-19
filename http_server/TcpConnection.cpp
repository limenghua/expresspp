#include "TcpConnection.h"
#include <boost/asio/spawn.hpp>
#include <iostream>

using namespace expresspp;

TcpConnection::TcpConnection(io_service & service, RecvHandler handler):
_io_service(service),
_socket(service),
_handler(handler)
{
}

void TcpConnection::Start()
{
	auto self = shared_from_this();

	boost::asio::spawn(_io_service,
		[this, self](boost::asio::yield_context yield)
	{
		try
		{
			char data[1024];
			for (;;)
			{
				std::size_t n = _socket.async_read_some(boost::asio::buffer(data), yield);
				if (_handler)
				{
					_handler(_socket, data, n);
				}
			}
		}
		catch (std::exception& e){
			std::cerr << e.what() << std::endl;
			_socket.close();
			//timer_.cancel();
		}
		catch (...){
			_socket.close();
			//timer_.cancel();
		}
	});
}



