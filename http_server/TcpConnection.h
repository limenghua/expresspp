#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <functional>

namespace expresspp{

class TcpConnection:public std::enable_shared_from_this<TcpConnection>
{
public:
	using io_service = boost::asio::io_service;
	using socket = boost::asio::ip::tcp::socket;
	using RecvHandler = std::function<void(socket &, const char *, size_t)>;
	using Pointer =  std::shared_ptr<TcpConnection>;
	using WeakPointer = std::weak_ptr<TcpConnection>;

	static Pointer Create(io_service & service, RecvHandler handler){
		return Pointer(new TcpConnection(service,handler));
	}

	void Start();

	socket & Socket(){
		return _socket;
	}
protected:
	TcpConnection(io_service & service, RecvHandler handler);

private:
	socket _socket;
	RecvHandler _handler;
	io_service & _io_service;
	
};

}//namespace expresspp


