#pragma once

#include <memory>

namespace expresspp{

    class Expresspp;
	class Request;
	typedef std::shared_ptr<Request> RequestPtr;
	class Response;
	typedef std::shared_ptr<Response> ResponsePtr;
	class ErrorCode;
	typedef std::shared_ptr<ErrorCode> ErrorCodePtr;

    class IMiddleWare
    {
    public:
        typedef std::shared_ptr<IMiddleWare> Pointer;
        typedef std::weak_ptr<IMiddleWare> WeakPointer;

        virtual void Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error) = 0;
        virtual void After(RequestPtr req, ResponsePtr res, ErrorCodePtr error) = 0;
        virtual void Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error) = 0;
        virtual ~IMiddleWare(){};
    };
    typedef IMiddleWare::Pointer MiddleWarePtr;

    class IModule
    {
    public:
        typedef std::shared_ptr<IModule> Pointer;
        typedef std::weak_ptr<IModule> WeakPointer;

        virtual void Initialize(Expresspp & app) = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
    };
    typedef IModule::Pointer ModulePtr;

} //namespace expresspp{