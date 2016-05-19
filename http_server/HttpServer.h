#pragma once

#include "TcpServer.h"
#include "Request.h"
#include "Response.h"
#include "ErrorCode.h"
#include <functional>
#include <memory>

#include <boost/asio.hpp>

namespace expresspp {

	typedef std::function<void(RequestPtr, ResponsePtr,ErrorCodePtr)> RequestHandler;
	class HttpServer :public std::enable_shared_from_this<HttpServer>
	{
	protected:
		HttpServer(RequestHandler handler);
	public:
		typedef std::shared_ptr<HttpServer> Pointer;
		typedef std::weak_ptr<HttpServer> WeakPointer;

		static Pointer Create(RequestHandler handler) {
			return Pointer(new HttpServer(handler));
		}

		Pointer Listen(unsigned short port);

		void Run();

		boost::asio::io_service & get_io_service()
		{
			return _tcpServer->get_io_service();
		}

	private:
		RequestHandler _requestHandler;
		TcpServerPtr   _tcpServer;
		
	};

	typedef HttpServer::Pointer HttpServerPtr;

}//namespace expresspp
