#include<gtest\gtest.h>
#include<expresspp\request.hpp>
#include<expresspp\response.hpp>

TEST(request, defualt_constructor)
{
	expresspp::request request;
	expresspp::response res;

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

	bool exception = false;
	size_t pos = 0;

	try {
		pos = request.consume(raw.c_str(), raw.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	EXPECT_FALSE(exception);
	EXPECT_EQ(pos, 41);
	EXPECT_TRUE(request.ready());
	EXPECT_TRUE(request.get_version() == "HTTP/1.1");
	EXPECT_TRUE(request.get_method() == "GET");
	EXPECT_TRUE(request.get_uri() == "/");
	EXPECT_TRUE(request.get_header("Host") == "www.example.com");
}

TEST(request, constructor)
{
	expresspp::websocketpp_request r;

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

	bool exception = false;
	size_t pos = 0;

	try {
		pos = r.consume(raw.c_str(), raw.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	EXPECT_FALSE(exception);
	EXPECT_TRUE(pos == 41);
	EXPECT_TRUE(r.ready());
	EXPECT_TRUE(r.get_version() == "HTTP/1.1");
	EXPECT_TRUE(r.get_method() == "GET");
	EXPECT_TRUE(r.get_uri() == "/");
	EXPECT_TRUE(r.get_header("Host") == "www.example.com");

	expresspp::request request(r);
	EXPECT_TRUE(request.ready());
	EXPECT_TRUE(request.get_version() == "HTTP/1.1");
	EXPECT_TRUE(request.get_method() == "GET");
	EXPECT_TRUE(request.get_uri() == "/");
	EXPECT_TRUE(request.get_header("Host") == "www.example.com");
}