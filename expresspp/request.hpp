#ifndef EXPRESSPP_REQUEST_HPP
#define EXPRESSPP_REQUEST_HPP

#include<websocketpp\http\request.hpp>
#include <unordered_map>
#include <boost/any.hpp>
#include <exception>

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

		std::string get_path()
		{
			std::string path = get_uri();

			size_t pos = path.find_first_of('?');
			if (pos != std::string::npos){
				path = path.substr(0, pos);
			}

			if (path != "/"&&(path.length()>0)&&(path[path.length() - 1] == '/')){ //ÒÆ³ý×îºóµÄ'/'·û
				path = path.substr(0, path.length() - 1);
			}

			return path;
		}

		template<class T>
		void set(const std::string &key, T && value)
		{
			m_settings.emplace(key, std::forward<T>(value));
		}

		
		template<class T>
		T get(const std::string &key)
		{
			auto it = m_settings.find(key);
			if (it == m_settings.end())
			{
				throw std::exception("No such key.");
			}

			return boost::any_cast<T>(it->second);
		}

		bool has(const std::string & key){
			return (m_settings.find(key) != m_settings.end());
		}

	private:
		std::unordered_map<std::string, boost::any> m_settings;
	};
}



#endif //EXPRESSPP_REQUEST_HPP    