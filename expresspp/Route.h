#pragma once
#include "http_server\Response.h"
#include "IMiddleWare.h"
#include "ArgsParserFactory.h"
#include "function_traits.hpp"
#include <map>
#include <functional>

namespace expresspp{

	typedef std::function<void(RequestPtr, ResponsePtr, ErrorCodePtr)> RequestHandler;
	class Router :
		public IMiddleWare
	{
	public:
		typedef std::shared_ptr<Router> Pointer;
		typedef std::weak_ptr<Router> WeakPointer;
		static Pointer Create(){
			return Pointer(new Router());
		}
	public:
		//IMiddleWare实现
		virtual void Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override{};
		virtual void After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override{};
		virtual void Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
	public:

		//注册路由
		template<typename Function>
		void Route(const std::string & path, Function f){
			function_traits<Function>::tuple_type *tup = nullptr;
			RouteImp(path, f, *tup);
		}

		template<typename ArgsTupleType,typename Function>
		void RouteImp(const std::string & path, Function f, ArgsTupleType &){
			//根据路由地址模式查找一个解析器			
			auto exuter = [path, f](RequestPtr req, ResponsePtr res, ErrorCodePtr error){
				auto parser = ArgsParserFactory::CreateParser(path);
				function_traits<Function>::tuple_type tup;
				parser->Parse(req);
				*parser >> tup;
				auto result = apply(f, tup);
				*res << result;
				res->End();
			};

			std::string routePath = ArgsParserFactory::CreateParser(path)->GetRoute();
			RegisterHandler(routePath, exuter);
		}
		template<typename Function>
		void RouteImp(const std::string & path, Function f, std::tuple<RequestPtr, ResponsePtr, ErrorCodePtr> & )
		{
			auto exuter = [f](RequestPtr req, ResponsePtr res, ErrorCodePtr error){
				f(req, res,error);
				res->End();
			};
			RegisterHandler(path, exuter);
		}
		template<typename Function>
		void RouteImp(const std::string & path, Function f, std::tuple<RequestPtr, ResponsePtr> & )
		{
			auto exuter = [f](RequestPtr req, ResponsePtr res, ErrorCodePtr error){
				f(req, res);
				res->End();
			};
			RegisterHandler(path, exuter);
		}

	protected:
		Router();
	private:
		std::string GetRoutePath(const std::string &path){
			return path;
		}
		void RegisterHandler(const std::string & route, RequestHandler handler){
			handlers[route] = handler;
		}
	private:
		std::map<std::string, RequestHandler> handlers;
	};

	typedef Router::Pointer RouterPtr;

}