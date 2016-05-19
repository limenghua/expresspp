#include "Route.h"
#include "http_server\Request.h"
using namespace expresspp;

Router::Router()
{
}

void Router::Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
	std::string path = req->GetFullPath();
	bool handled = false;

	while (!path.empty()){
		auto it = handlers.find(path);
		if (it != handlers.end()){
			it->second(req, res, error);
			handled = true;
			break;
		}
		auto pos = path.rfind('/');
		if (pos != path.npos){
			path = path.substr(0, pos);
		}
	}

	//�������û�б�����,����"/"��������·��
	if (!handled){
		auto it = handlers.find("/");
		if (it != handlers.end()){
			it->second(req, res, error);
			handled = true;
		}
	}
}
