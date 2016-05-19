#include "Expresspp.h"
#include "http_server\utils.h"
#include "Cookie.h"
#include <iostream>

using namespace expresspp;

CookieParser::CookieParser()
{
}


void CookieParser::Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    
    const std::string & cookieStr = req->GetHeader("Cookie"); 
    auto cookie = _Parse(cookieStr);

    //Request 和 Response公用同一个Cookie对象，会不会有问题
    req->Set<Cookie>(cookie);
    res->Set<Cookie>(cookie);
}

void CookieParser::After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    auto cookie = res->Get<Cookie>();
    if (!cookie){
        std::cout << "conn't find cookie from response object" << std::endl;
        return;
    }
    auto cookieStr = _Serialize(cookie);
    res->SetHeader("Set-Cookie", cookieStr);

}

void CookieParser::Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{

}

CookiePtr CookieParser::_Parse(const std::string & cookieStr)
{
    auto cookie = Cookie::Create();
    auto elements = StringSplit(cookieStr, ';');
    for (auto elem : elements){
        auto keyValues = StringSplit(elem, '=');
        if (keyValues.size() == 2){
            cookie->data[keyValues[0]] = keyValues[1];
        }
    }
    return cookie;
}

std::string CookieParser::_Serialize(CookiePtr cookie)
{
    std::stringstream ss;
    if (!cookie || cookie->data.empty()){
        return ss.str();
    }
    auto it = cookie->data.begin();
    if (it != cookie->data.end()){
        ss << it->first << "=" << it->second;
        it++;
    }

    for (; it != cookie->data.end();it++)
    {
        ss << ";";
        ss << it->first << "=" << it->second;
    }

    std::string str = ss.str();
    
    return str;
}
