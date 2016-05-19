#include "example\config.h"

#include "expresspp\UrlQuery.h"

using namespace expresspp;

void ExampleQueryString()
{
    cout << "Example QueryString" << endl;

    auto handler =
        [](RequestPtr req, ResponsePtr res, ErrorCodePtr errorCode){

        std::string html = "<html><body>";

        auto urlQueryData = req->Get<UrlQueryData>();
        if (urlQueryData){
            html = html + "<p> path=" + urlQueryData->path + "</p>";
            html += "</p>query strings:</p>";
            for (auto item : urlQueryData->data){
                html = html + "<p>" + item.first + ":" + item.second + "</p>";
            }
            html += "</body></html>";
            res->Write(html);
        }

        res->SetStatusCode(200);
        res->End();
    };
    try{
        auto app = expresspp::Expresspp::CreateApplication();
        app->Use(UrlQuery::Create())
            ->Use(handler)
            ->Listen(8080)
            ->Run();
    }
    catch (...){
        std::cout << "exception" << std::endl;
    }
}