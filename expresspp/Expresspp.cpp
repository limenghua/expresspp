#include "expresspp\Expresspp.h"
#include "expresspp\IMiddleWare.h"
#include "Route.h"
#include <functional>

using namespace expresspp;
using namespace std::placeholders;

Expresspp::Expresspp()
    :io_service()
{
}

void Expresspp::Initialize()
{
    if (!httpServer){
        httpServer = HttpServer::Create( 
            std::bind(&Expresspp::Handle,
            shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2,
            std::placeholders::_3));
    }
	router = Router::Create();
	Use((MiddleWarePtr)router);
}

Expresspp::Pointer Expresspp::instance = Expresspp::Pointer();
Expresspp::Pointer Expresspp::CreateApplication(){
    if (!instance){
        instance = Pointer(new Expresspp());
        instance->Initialize();
    }
    return instance;
}

Expresspp::Pointer Expresspp::Listen(unsigned short port)
{
    httpServer->Listen(port);
    return shared_from_this();
}
Expresspp::Pointer Expresspp::Run(){
	httpServer->Run();
    return shared_from_this();
}

void Expresspp::Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    for (auto before : befores){
        before(req, res, error);
    }
    for (auto handle : handlers){
        if (res->IsComplated()){
            break;
        } 
        handle(req, res, error);
    }
    for (auto after : afters){
        after(req, res, error);
    }
}

Expresspp::Pointer Expresspp::Use(MiddleWarePtr middleWare)
{
    Before(std::bind(
        &IMiddleWare::Before,
        middleWare,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3));

    After(std::bind(
        &IMiddleWare::After,
        middleWare,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3));

    RegisterHandler(std::bind(
        &IMiddleWare::Handle,
        middleWare,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3));

    return shared_from_this();
}
Expresspp::Pointer Expresspp::Use(ModulePtr module)
{
    module->Initialize(*this);
    modules.push_back(module);
    return shared_from_this();
}