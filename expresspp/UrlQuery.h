#pragma once

#include "IMiddleWare.h"
#include <map>

namespace expresspp{

    struct UrlQueryData{
        std::string path;
        std::map<std::string, std::string> data;

        static std::shared_ptr<UrlQueryData> Create(){
            return std::make_shared<UrlQueryData>();
        }
    };

    class UrlQuery :
        public IMiddleWare
    {
    private:
        UrlQuery();
    public:
        static Pointer Create(){
            return Pointer(new UrlQuery());
        }
        virtual void Before(RequestPtr req,ResponsePtr res,ErrorCodePtr error)override;
        virtual void After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
        virtual void Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;

        ~UrlQuery() override;
    };

};//namespace expresspp{