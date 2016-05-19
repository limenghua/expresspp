#pragma once


#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include "HttpServer.h"
#include <memory>


using boost::asio::io_service;
using boost::asio::ip::tcp;

namespace expresspp{

	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		typedef std::shared_ptr<Connection> Pointer;
		typedef std::weak_ptr<Connection> WeakPointer;

		static Pointer Create(io_service & service, RequestHandler handler){
			return Pointer(new Connection(service, handler));
		}

		void Start();
		
		tcp::socket & Socket(){
			return socket_;
		}

	protected:
		Connection(io_service & service, RequestHandler handler);
	private:
		void DoWork();
	private:
		RequestHandler handler_;
		io_service & service_;
		tcp::socket socket_;
		boost::asio::steady_timer timer_;
	};
    typedef Connection::Pointer ConnectionPtr;
}


