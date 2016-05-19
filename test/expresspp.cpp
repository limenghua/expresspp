#include "expresspp\expresspp.h"
#include <gtest\gtest.h>

using namespace expresspp;

TEST(Expresspp, Constructor)
{
    auto app = Expresspp::CreateApplication();
    auto app2 = Expresspp::CreateApplication();

    ASSERT_EQ(app, app2);
}

TEST(Expresspp, Listen)
{
    auto app = Expresspp::CreateApplication();
    
    decltype(app) rApp;

    ASSERT_NO_THROW(rApp = app->Listen(80));

    ASSERT_EQ(app, rApp);
}

TEST(Expresspp, Before)
{
    auto beforeHandler = [](RequestPtr req, ResponsePtr, ErrorCodePtr error){};
    auto app = Expresspp::CreateApplication();
    decltype(app) ret;

    ASSERT_NO_THROW(ret = app->Before(beforeHandler));
    ASSERT_EQ(ret, app);
}

TEST(Expresspp, After)
{
    auto afterHandler = [](RequestPtr req, ResponsePtr, ErrorCodePtr error){};
    auto app = Expresspp::CreateApplication();
    decltype(app) ret;

    ASSERT_NO_THROW(ret = app->After(afterHandler));
    ASSERT_EQ(ret, app);
}

TEST(Expresspp, Use)
{
	auto api = [](int a, int b)->int{return a + b; };
	auto app = Expresspp::CreateApplication();
	decltype(app) ret;

	ASSERT_NO_THROW(ret = app->Use("/add", api));
	ASSERT_EQ(ret, app);
}

TEST(Expresspp, UseRequestHandle)
{
	auto api = [](RequestPtr req,ResponsePtr res,ErrorCodePtr error){
		res->End("Test");
	};

	auto api2 = [](RequestPtr req, ResponsePtr res)
	{

	};

	auto app = Expresspp::CreateApplication();
	decltype(app) ret;

	ASSERT_NO_THROW(ret = app->Use("/add", api));
	ASSERT_NO_THROW(ret = app->Use("/sub", api2));
	ASSERT_EQ(ret, app);
}