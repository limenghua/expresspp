#pragma once
#include <boost/asio.hpp>
#include "http_server\HttpServer.h"
#include "Route.h"

#include <memory>
#include <list>


namespace expresspp{

    class IMiddleWare;
    typedef std::shared_ptr<IMiddleWare> MiddleWarePtr;
    class IModule;
    typedef std::shared_ptr<IModule> ModulePtr;

    class Expresspp:public std::enable_shared_from_this<Expresspp>
    {
    public:
        typedef std::shared_ptr<Expresspp> Pointer;
        typedef std::weak_ptr<Expresspp> WeakPtr;
    private:
        Expresspp();
        Expresspp(const Expresspp&) = delete;
        Expresspp & operator= (const Expresspp &) = delete;
    public:
        static Pointer CreateApplication();
        Pointer Listen(unsigned short port);
        Pointer Run();
		Pointer Start();
		Pointer Stop();

		boost::asio::io_service & GetIoService(){
			return io_service;
		}
    public:
        Pointer Before(RequestHandler before){
            befores.push_back(before);
            return shared_from_this();
        }
        Pointer After(RequestHandler after){
            afters.push_back(after);
            return shared_from_this();
        }
        Pointer RegisterHandler(RequestHandler handler){
            handlers.push_back(handler);
            return shared_from_this();
        }

        Pointer Use(RequestHandler handler){
            return RegisterHandler(handler);
        }

        Pointer Use(MiddleWarePtr middleWare);
        Pointer Use(ModulePtr module);

		template<typename Function>
		Pointer Use(const std::string & path, Function && f){
			router->Route(path, std::forward<Function>(f));
			return shared_from_this();
		}

		template<typename Function>
		Pointer Route(const std::string & path, Function && f){
			return Use(path, std::forward<Function>(f));
		}

    private:
        void Initialize();
        void Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error);

    private:
        static Pointer instance;
        boost::asio::io_service io_service;
        HttpServerPtr httpServer;
        std::list<RequestHandler> handlers;
        std::list<RequestHandler> befores;
        std::list<RequestHandler> afters;
        std::list<ModulePtr> modules;
		RouterPtr router;
    };
    typedef Expresspp::Pointer ExpressppPtr;
    typedef ExpressppPtr Application;

};//namespace expresspp{

