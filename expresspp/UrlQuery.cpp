#include "Expresspp.h"
#include "UrlQuery.h"
#include "http_server\utils.h"
using namespace expresspp;

UrlQuery::UrlQuery()
{
}

UrlQuery::~UrlQuery()
{
}


void UrlQuery::Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    auto url = req->GetUrl();
    auto queryData = UrlQueryData::Create();
    auto items = StringSplit(url,'?');
    if (items.size() == 0){
        return;
    }
    queryData->path = items[0];
    if (items.size() == 2){
        auto queryString = items[1];
        queryData->data = ParseQueryString(queryString);
    }    
    req->Set<UrlQueryData>(queryData);
}
void UrlQuery::After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    ;
}
void UrlQuery::Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    ;
}