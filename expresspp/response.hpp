#ifndef EXPRESSPP_RESPONSE_HPP
#define EXPRESSPP_RESPONSE_HPP

#include <websocketpp\http\response.hpp>
#include <sstream>

namespace expresspp{
	typedef websocketpp::http::parser::response  websocketpp_response;
	class response : public websocketpp_response
	{
	public:
		response(const websocketpp_response &res) : websocketpp_response(res), m_sstream(), m_isdone(false)
		{
		}
		response() : m_sstream(), m_isdone(false)
		{
		}

		void end(){
			m_isdone = true;
			set_body(get_body() + m_sstream.str());
		}

		void end(const std::string & body){
			m_sstream << body;
			end();
		}

		bool is_done(){
			return m_isdone;
		}

		template<class Type>
		response & operator << (Type & value){
			m_sstream << value;
			return *this;
		}

	private:
		std::stringstream m_sstream;
		bool m_isdone;
	};

}//namespace expresspp

#endif //EXPRESSPP_RESPONSE_HPP