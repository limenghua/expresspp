#include "expresspp\Route.h"

#include <gtest\gtest.h>

using namespace expresspp;

TEST(Route, construct)
{
	auto router = Router::Create();
	ASSERT_TRUE(router);
}

TEST(Route, route)
{
	auto router = Router::Create();
	router->Route("/hello", [](int a, int b)
		->std::string
	{
		std::cout << "a and b" << a << b << std::endl;
		return std::string("hello world");
	});
}