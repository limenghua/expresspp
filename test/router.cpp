#include<gtest\gtest.h>
#include<expresspp\request.hpp>
#include<expresspp\response.hpp>
#include<expresspp\router.hpp>

TEST(router, route)
{
	expresspp::router router;
	expresspp::request req;
	expresspp::response res;

	router.route("hello/", [](expresspp::request & req, expresspp::response & res){

	});
}

TEST(router, dispatch)
{
	expresspp::router router;
	expresspp::request req;
	expresspp::response res;
	router.dispatch(req, res);
}