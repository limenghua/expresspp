#include <boost/asio.hpp>

#include "../expresspp/expresspp.h"
#include "../http_server/helper.h"
#include <iostream>
#include <thread>
#include <string>

using  std::cout;
using std::cin;
using std::endl;

using expresspp::HttpServer;
using expresspp::RequestPtr;
using expresspp::ResponsePtr;
using expresspp::ErrorCodePtr;

using namespace boost;

int main()
{
    //boost::asio::io_service io_service;

    auto api = 
        [](int a,int b)->int{
            return a+b;
        };
	auto api2 =
		[](int a, int b)->int{
		return a * b;
	};

    try{
        auto app = expresspp::Expresspp::CreateApplication();
        app->Use("/add",api)
			->Use("/add/new",api2)
			->Route("/", []()->std::string{
					return std::string("Hello World\n");
				})
            ->Listen(8080)
            ->Run();
    }
	catch (std::exception & e){
		std::cout << "exception:" <<e.what()<< std::endl;
	}
    catch (...){
        std::cout << "exception" << std::endl;
    }
}