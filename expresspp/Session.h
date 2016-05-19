#pragma once

#include "IMiddleWare.h"
#include "http_server\AnyContainer.h"

namespace expresspp{
    
    class Session :public PTree
    {
    public:
        typedef std::shared_ptr<Session> Pointer;
        typedef std::weak_ptr<Session> WeakPointer;

        Session(const std::string & id)
            :sessionId(id)
        {
        }

        static Pointer Create(const std::string & id){
            return std::make_shared<Session>(id);
        }

        const std::string & GetId(){
            return sessionId;
        }

        double GetVisiteTime(){
            return visitTime;
        }
        void SetVisiteTime(double time){
            visitTime = time;
        }

    private:
        std::string sessionId;
        double      visitTime;
    };
    typedef Session::Pointer SessionPtr;

    class SessionManager:public IMiddleWare
    {
        SessionManager();
        SessionManager(const SessionManager&) = delete;
        SessionManager(const SessionManager&&) = delete;
        SessionManager & operator = (const SessionManager&) = delete;
    public:
        typedef std::shared_ptr<SessionManager> Pointer;
        typedef std::weak_ptr<SessionManager> WeakPointer;

        static IMiddleWare::Pointer Create(){
            return Instance();
        }
        static Pointer Instance(){
            if (!instance){
                instance = Pointer(new SessionManager());
            }
            return instance;
        }
    public:
        void Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
        void After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
        void Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)override;
    public:
        SessionPtr CreateSession();
        SessionPtr GetSession(const std::string & id);
        void       DeleteSession(const std::string & id);
        void Update();

    private:
        static Pointer instance;
        std::map<std::string, SessionPtr> sessions;
        double  timeout;
    };

    typedef SessionManager::Pointer SessionManagerPtr;

}

