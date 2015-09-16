#ifndef EXPRESSPP_REQUEST_HPP
#define EXPRESSPP_REQUEST_HPP

#include<websocketpp\http\request.hpp>

namespace expresspp
{
	typedef websocketpp::http::parser::request  websocketpp_request;
	class request : public websocketpp_request
	{
	public:
		request(const websocketpp_request &req) : websocketpp_request(req)
		{
		}
		request() = default;
	};
}



#endif //EXPRESSPP_REQUEST_HPP