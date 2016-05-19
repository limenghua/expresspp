#include "http_server\Request.h"
#include "ArgsParserFactory.h"

using namespace expresspp;

ParmsArgsParser::ParmsArgsParser(const std::string & pattern)
{
	this->pattern = pattern;
	ParseParmsName();
}

void ParmsArgsParser::ParseParmsName()
{

}

void ParmsArgsParser::Parse(RequestPtr req)
{
	std::string parm = req->GetFullPath();
	ss.str("");
	ss << 15 << " " << 20;
}

bool ParmsArgsParser::Match(const std::string & route){
	return true;
}

std::string ParmsArgsParser::GetRoute(){
	return pattern;
}

ArgsParserFactory::ArgsParserFactory()
{
}


ArgsParserFactory::~ArgsParserFactory()
{
}
