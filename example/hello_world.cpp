#include "example\config.h"

void ExampleHelloWorld()
{
    //boost::asio::io_service io_service;

    auto handler =
        [](RequestPtr req, ResponsePtr res, ErrorCodePtr errorCode){
        res->SetStatusCode(200);
        res->End("Hello World");
    };
    try{
        auto app = expresspp::Expresspp::CreateApplication();
        app->Use(handler)
            ->Listen(8080)
            ->Run();
    }
    catch (...){
        std::cout << "exception" << std::endl;
    }
}