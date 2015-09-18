#include <gtest\gtest.h>

#include <expresspp\expresspp.hpp>

using namespace expresspp;


TEST(expresspp, expresspp)
{
	server app;
}

TEST(expresspp, listen)
{
	server app;
	
	server & ret = app.listen(8000);
	ASSERT_EQ(&app, &ret);
}

TEST(expresspp, use)
{
	server app;
	app.use([](request & req, response & res){
		res.end("hello");
	});
}

TEST(expresspp, route)
{
	server app;
	app.route("hello", [](request & req, response & res){
		res.end("hello");
	});
}

TEST(expresspp, set_get)
{
	server app;
	app.set("int_value1", 1);
	app.set("int_value2", 2);
	app.set("bool_value1", true);
	app.set("string_value1", std::string("test_string"));

	EXPECT_NO_THROW(app.get<int>("int_value1"));
	EXPECT_NO_THROW(app.get<int>("int_value2"));
	EXPECT_NO_THROW(app.get<bool>("bool_value1"));
	EXPECT_NO_THROW(app.get<std::string>("string_value1"));

	EXPECT_EQ(app.get<int>("int_value1"), 1);
	EXPECT_EQ(app.get<int>("int_value2"), 2);
	EXPECT_EQ(app.get<bool>("bool_value1"), true);
	EXPECT_EQ(app.get<std::string>("string_value1"), std::string("test_string"));

	EXPECT_ANY_THROW(app.get<std::string>("int_value1"));
	EXPECT_ANY_THROW(app.get<int>("other_value"));
}