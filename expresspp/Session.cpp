#define _SCL_SECURE_NO_WARNINGS
#include "http_server\Response.h"
#include "http_server\Request.h"
#include "expresspp\Session.h"
#include "expresspp\Cookie.h"

#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_generators.hpp>
#include <boost\uuid\uuid_io.hpp>




using namespace expresspp;

std::string GenerateSessionId(){
    boost::uuids::uuid u = boost::uuids::string_generator()("{0123456789abcdef0123456789abcdef}");
    return std::string("abcdef");
    //return boost::uuids::to_string(u);
}
double GetTime(){

    return 0;
}

SessionManagerPtr SessionManager::instance = nullptr;

SessionManager::SessionManager()
    :timeout(3.0)
{
}

SessionPtr SessionManager::CreateSession(){
    std::string id = GenerateSessionId();
    SessionPtr session = Session::Create(id);
    session->SetVisiteTime(GetTime());

    sessions[id] = session;
    return session;
}

SessionPtr SessionManager::GetSession(const std::string &id)
{
    auto it = sessions.find(id);
    if (it == sessions.end()){
        return SessionPtr();
    }
    return it->second;
}

void SessionManager::DeleteSession(const std::string & id)
{
    sessions.erase(id);
}

void SessionManager::Update()
{
    double time = GetTime();
    for (auto it = sessions.begin(); it != sessions.end();){
        double aliveTime = time - it->second->GetVisiteTime();
        auto thisItrator = it;
        it++;
        if (aliveTime > timeout){
            sessions.erase(thisItrator);
        }
    }
}

void SessionManager::Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    auto cookie = req->Get<Cookie>();
    if (!cookie){
        return;
    }
    std::string sessionId = cookie->data["SESSIONID"];
    auto session = sessions[sessionId];
    if (!session){
        session = CreateSession();
        cookie->data["SESSIONID"] = session->GetId();
    }
    req->Set<Session>(session);
}
void SessionManager::After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    //do nothing
}
void SessionManager::Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
    //do nothing
}