#pragma once
#include "IMiddleWare.h"

namespace expresspp{

    struct Cookie
    {
        typedef std::shared_ptr<Cookie> Pointer;
        typedef std::weak_ptr<Cookie> WeakPtr;
        static Pointer Create(){
            return std::make_shared<Cookie>();
        }
        std::string & operator[] (const std::string & key){
            return data[key];
        }

        std::map<std::string, std::string> data;
    };
    typedef Cookie::Pointer CookiePtr;

    class CookieParser :
        public IMiddleWare
    {
    public:
        static Pointer Create(){
            return Pointer(new CookieParser());
        };
    public:
        void Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
        void After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
        void Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
    protected:
        CookieParser();


    //Ϊ�˲��Է��㣬��˽�к�������public���ⲿ��Ҫʹ����
    //����ֻ������̽���ֱ�̷�ʽ����������д��������
    public:
        CookiePtr _Parse(const std::string & cookieStr);
        std::string _Serialize(CookiePtr cookie);
    };



}//namespace expresspp{