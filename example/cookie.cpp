#include "example\config.h"
#include <boost/lexical_cast.hpp>

#include "expresspp\Cookie.h"
#include "expresspp\Session.h"
#include "expresspp\UrlQuery.h"
#include "expresspp\StaticFile.h"

using namespace expresspp;

template<typename Type>
Type FromString(const std::string & str,Type FialsValue){
    try
    {
        return(boost::lexical_cast<Type>(str));
    }
    catch (const bad_lexical_cast &)
    {
        return FialsValue;
    }
}

template<typename Type>
std::string ToString(Type value){
    return boost::lexical_cast<std::string>(value);
}

void ExampleCookie()
{
    cout << "Example Cookie" << endl;

    auto handler =
        [](RequestPtr req, ResponsePtr res, ErrorCodePtr errorCode){

        auto cookie = req->Get<Cookie>();
        
        //ÕÒ²»µ½cookie
        if (!cookie){
            res->SetStatusCode(200);
            res->End("Conn't find cookie object");
            return;
        }

        auto & cookieData = cookie->data;

        int visitTimes = 0;
        if (cookieData.find("TheTimes") == cookieData.end()){
            cookieData["TheTimes"] = "1";
            cookieData["MyTimes"] = "2";
            visitTimes = 1;
        }
        else{
            visitTimes = FromString(cookieData["TheTimes"],0);
            visitTimes++;
            cookieData["TheTimes"] = ToString(visitTimes);
            cookieData["MyTimes"] = ToString(visitTimes+1);
        }

        std::string html = "<html><body>";

        html = html + "<p> you visit site " + cookieData["TheTimes"] + "times</p>";
        html += "</body></html>";

        res->Write(html);
        res->SetStatusCode(200);
        res->End();
    };
    try{
        auto app = expresspp::Expresspp::CreateApplication();
		app->Use(UrlQuery::Create())
			->Use(CookieParser::Create())
			->Use(SessionManager::Create())
			->Use(StaticFile::Create("d:/public"))
			->Use(handler)
            ->Listen(8080)
            ->Run();
    }
    catch (...){
        std::cout << "exception" << std::endl;
    }
}