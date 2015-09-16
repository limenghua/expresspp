#include <gtest\gtest.h>

#include <expresspp\expresspp.hpp>

using namespace expresspp;


TEST(expresspp, expresspp)
{
	server app = server();
}

TEST(expresspp, listen)
{
	server app = server();
	
	server & ret = app.listen(8000);
	ASSERT_EQ(&app, &ret);
}