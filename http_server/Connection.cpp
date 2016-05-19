#include "Connection.h"

#include <boost/asio/spawn.hpp>
#include <iostream>

using namespace expresspp;

Connection::Connection(io_service & service, RequestHandler handler)
	:service_(service),
	handler_(handler),
	socket_(service),
	timer_(service)
{
}

void Connection::Start()
{
	auto self = shared_from_this();

	boost::asio::spawn(service_,
		[this, self](boost::asio::yield_context yield)
	{
		try
		{
			char data[1024];
			auto req = Request::Create();
			auto res = Response::Create();
			auto error = ErrorCode::Create();

			for (;;)
			{
				std::size_t n = socket_.async_read_some(boost::asio::buffer(data), yield);
				req->Consume(data, n);
				if (req->IsReady()){

					if (req->ShouldKeepAlive()){
						res->SetHeader("Connection","Keep-Alive");
					}

					handler_(req, res, error);

					if (!res->IsComplated()){
						res->End();
					}

					auto &buffer = res->GetBuffer();
                    std::string headStr = res->GetHeaderString();       

					boost::asio::async_write(socket_, 
						boost::asio::buffer(headStr), yield);
					boost::asio::async_write(socket_, buffer,
						boost::asio::transfer_exactly(buffer.size()), yield);

                    if (req->ShouldKeepAlive()){
                        res->Reset();
                        req->Reset();
                        error->Reset();
                    }
                    else{
                        socket_.close();
                        break;
                    }
				}
			}
		}
		catch (std::exception& e){
			std::cout << e.what() << std::endl;
			socket_.close();
			//timer_.cancel();
		}
		catch (...){
			socket_.close();
			//timer_.cancel();
		}
	});
}

