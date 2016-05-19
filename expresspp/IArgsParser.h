#pragma once

#include <memory>
#include <sstream>

namespace expresspp
{
	class Request;
	typedef std::shared_ptr<Request> RequestPtr;

	template <typename Tuple, std::size_t N>
	struct TupleOutput{
		static void output(std::stringstream & ss, Tuple & tup){
			TupleOutput<Tuple, N - 1>::output(ss, tup);
			ss >> std::get<N - 1>(tup);
		}
	};
	template<typename Tuple>
	struct TupleOutput <Tuple, 0 > {
		static void output(std::stringstream & ss, Tuple & tup){
			//ss >> std::get<0>(tup);
		}
	};

	template<typename ...Args>
	std::stringstream & operator >> (std::stringstream & ss, std::tuple<Args...> & tup){
		TupleOutput<decltype(tup), sizeof...(Args)>::output(ss, tup);
		return ss;
	}

	class ArgsParser
	{
	public:
		typedef std::shared_ptr<ArgsParser> Pointer;
		typedef std::weak_ptr<ArgsParser> WeakPointer;
	public:
		virtual void Parse(RequestPtr req) = 0;
		virtual bool Match(const std::string & pattern) = 0;
		virtual std::string GetRoute() = 0;

		template<typename ...Args>
		ArgsParser & operator >> (std::tuple<Args...> &tup){
			ss >> tup;
			return *this;
		}


	protected:
		std::stringstream ss;
		std::string pattern;
	};
	typedef ArgsParser::Pointer ArgsParserPtr;


}//namespace expresspp
