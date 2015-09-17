#ifndef EXPRESSPP_ROUTER_HPP
#define EXPRESSPP_ROUTER_HPP

#include <expresspp/response.hpp>
#include <expresspp/request.hpp>

#include <string>
#include <functional>
#include <map>

namespace expresspp{
	typedef std::function<void(request & req, response & res)> handle_function;

	class router
	{
	public:

		void route(const std::string & path, handle_function fun){
			handle_functions.emplace(path, fun);
		}


		void dispatch(request & req, response & res){
			std::string path = req.get_uri();
			typedef handle_container::iterator iterator;
			std::pair<iterator, iterator> result = handle_functions.equal_range(path);
			for (auto it = result.first; it != result.second; it++)
			{
				if (it->second){
					it->second(req, res);
				}
			}

		}

	private:
		typedef std::multimap<std::string, handle_function> handle_container;
		handle_container handle_functions;
	};
	
} //namespace expresspp

#endif //EXPRESSPP_ROUTER_HPP