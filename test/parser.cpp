#include "expresspp\ArgsParserFactory.h"
#include "http_server\Request.h"
#include <string>
#include <gtest/gtest.h>
#include <tuple>


using namespace expresspp;

TEST(Parser, constructor)
{
	auto req = Request::Create();
	std::string path = "/hello";
	auto parser = ArgsParserFactory::CreateParser(path);
}

TEST(Parser, Output)
{
	auto req = Request::Create();
	std::string path = "/hello";
	auto parser = ArgsParserFactory::CreateParser(path);
	parser->Parse(req);
	std::tuple<int, std::string, double> tuple1;
	*parser >> tuple1;
}