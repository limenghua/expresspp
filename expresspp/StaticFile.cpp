#include "http_server\Response.h"
#include "http_server\Request.h"
#include <boost\lexical_cast.hpp>
#include "StaticFile.h"



using namespace expresspp;

StaticFile::StaticFile(const std::string & dir)
    :staticDir(dir)
{
}

void StaticFile::Before(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{

}
void StaticFile::After(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{

}
void StaticFile::Handle(RequestPtr req, ResponsePtr res, ErrorCodePtr error)
{
	if (staticDir.empty()){
		return;
	}

    std::string fullName = req->GetFullPath();
	if (fullName.rfind('/') == fullName.length() - 1){
		fullName = fullName + "index.htm";
    }

	fullName = staticDir + fullName;

	std::fstream fin(fullName, std::ios::binary | std::ios::in);
	if (!fin)
	{
		return ;
	}
	res->SetHeader("Content-Type", ContentType(fullName));

	char data[1024];
	while (!fin.eof())
	{
		fin.read(data, sizeof(data));
		res->Write(data, size_t(fin.gcount()));
	}

	res->End();
}