#pragma once

#include <functional>
#include <memory>

#include <boost/asio.hpp>

namespace expresspp{

	class TcpServer :public std::enable_shared_from_this<TcpServer>
	{
	public:
	using socket = boost::asio::ip::tcp::socket;
	using acceptor = boost::asio::ip::tcp::acceptor;
	using RecvHandler = std::function<void(socket &, const char *, size_t)>;
	using AcceptHander = std::function < void(socket &) > ;
	protected:
		TcpServer(RecvHandler handler);
		TcpServer(AcceptHander handler);
	public:
		typedef std::shared_ptr<TcpServer> Pointer;
		typedef std::weak_ptr<TcpServer> WeakPointer;

		static Pointer Create(RecvHandler handler){
			return Pointer(new TcpServer(handler));
		}
		static Pointer Create(AcceptHander handler){
			return Pointer(new TcpServer(handler));
		}

		Pointer SetAcceptHandler(AcceptHander handler){
			_acceptHandler = handler;
		}

		Pointer Listen(unsigned short port);

		void Run();

		boost::asio::io_service & get_io_service()
		{
			return _io_service;
		}
	private:
		boost::asio::io_service	 _io_service;
		acceptor  _acceptor;
		RecvHandler _recvHandler;
		AcceptHander _acceptHandler;
	};

	typedef TcpServer::Pointer TcpServerPtr;

} // namesapce expresspp