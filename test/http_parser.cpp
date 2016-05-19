#include "http_server\HttpParser.h"
#include "http_server\Request.h"
#include <gtest\gtest.h>

using expresspp::HttpParser;

TEST(HttpParser, Construct)
{
	auto parser = HttpParser::Create(HTTP_REQUEST);

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

	bool exception = false;
	size_t pos = 0;

	try {
		pos = parser->Consume(raw.c_str(), raw.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	ASSERT_TRUE(exception == false);
	ASSERT_EQ(pos, 41);
	ASSERT_EQ((parser->IsReady()), true);
	ASSERT_EQ(parser->GetMethod(), "GET");
	ASSERT_EQ(parser->GetHeader("Host"), "www.example.com");
	ASSERT_EQ((parser->GetUrl()), "/");

}

TEST(Request,basic_request_with_body)
{
    auto req = expresspp::Request::Create();

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\nContent-Length: 6\r\n\r\nabcdef";

	bool exception = false;
	size_t pos = 0;

	try {
		pos = req->Consume(raw.c_str(), raw.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	ASSERT_EQ(exception , false);
	ASSERT_EQ(pos, 66);
	ASSERT_TRUE(req->IsReady() == true);
	ASSERT_EQ(req->GetVersionMajor(), 1);
	ASSERT_EQ(req->GetMethod(), "GET");
	ASSERT_EQ(req->GetUrl(), "/");
	ASSERT_EQ(req->GetHeader("Host"), "www.example.com");
	ASSERT_EQ(req->GetHeader("Content-Length"), "6");
	ASSERT_EQ(req->GetBody(), "abcdef");
}


TEST(Request, basic_request_with_body_split) {

    auto req = expresspp::Request::Create();

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\nContent-Length: 6\r\n\r\nabc";
	std::string raw2 = "def";

	bool exception = false;
	size_t pos = 0;

	try {
		pos += req->Consume(raw.c_str(), raw.size());
		pos += req->Consume(raw2.c_str(), raw2.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	ASSERT_EQ(exception, false);
	ASSERT_EQ(pos, 66);
	ASSERT_TRUE(req->IsReady() == true);
    ASSERT_EQ(req->GetVersionMajor(), 1);
	ASSERT_EQ(req->GetMethod(), "GET");
	ASSERT_EQ(req->GetUrl(), "/");
	ASSERT_EQ(req->GetHeader("Host"), "www.example.com");
	ASSERT_EQ(req->GetHeader("Content-Length"), "6");
	ASSERT_EQ(req->GetBody(), "abcdef");
}


TEST(Request, trailing_body_characters) 
{
	auto req = expresspp::Request::Create();

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

	bool exception = false;
	size_t pos = 0;

	try {
		pos = req->Consume(raw.c_str(), raw.size());
	}
	catch (...) {
		exception = true;
	}

	ASSERT_EQ(exception, false);
	ASSERT_TRUE(pos == 41);
	ASSERT_TRUE(req->IsReady() == true);
    ASSERT_EQ(req->GetVersionMajor(), 1);
	ASSERT_TRUE(req->GetMethod() == "GET");
	ASSERT_TRUE(req->GetUrl() == "/");
	ASSERT_TRUE(req->GetHeader("Host") == "www.example.com");
}

TEST(Request, trailing_body_characters_beyond_max_lenth) {

	auto req = expresspp::Request::Create();

	/// Maximum size in bytes before rejecting an HTTP header as too big.
	size_t const max_header_size = 16000;
	/// Default Maximum size in bytes for HTTP message bodies.
	size_t const max_body_size = 32000000;

	std::string raw = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
	raw.append(max_header_size, '*');

	bool exception = false;
	size_t pos = 0;

	try {
		pos = req->Consume(raw.c_str(), raw.size());
	}
	catch (...) {
		exception = true;
	}

	ASSERT_EQ(exception, true);
	//ASSERT_EQ(pos , 0);
	ASSERT_TRUE(req->IsReady() == true);
    ASSERT_EQ(req->GetVersionMajor(), 1);
	ASSERT_TRUE(req->GetMethod() == "GET");
	ASSERT_TRUE(req->GetUrl() == "/");
	ASSERT_TRUE(req->GetHeader("Host") == "www.example.com");
}

TEST(Request, basic_split1) {
	auto req = expresspp::Request::Create();

	std::string raw = "GET / HTTP/1.1\r\n";
	std::string raw2 = "Host: www.example.com\r\n\r\n";

	bool exception = false;
	size_t pos = 0;

	try {
		pos += req->Consume(raw.c_str(), raw.size());
		pos += req->Consume(raw2.c_str(), raw2.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	ASSERT_EQ(exception, false);
	ASSERT_TRUE(pos == 41);
	ASSERT_TRUE(req->IsReady() == true);
    ASSERT_EQ(req->GetVersionMajor(), 1);
	ASSERT_TRUE(req->GetMethod() == "GET");
	ASSERT_TRUE(req->GetUrl() == "/");
	ASSERT_TRUE(req->GetHeader("Host") == "www.example.com");
}

TEST(Request, bad_method) {
	auto req = expresspp::Request::Create();

	std::string raw = "GE]T / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";

	bool exception = false;

	try {
		req->Consume(raw.c_str(), raw.size());
	}
	catch (...) {
		exception = true;
	}

	ASSERT_EQ(exception, true);
}

TEST(HttpParser, GetFullPath)
{
    auto req = expresspp::Request::Create();

    std::string raw = "GET /index.htm?name=limenghua HTTP/1.1\r\nHost: www.example.com\r\nContent-Length: 6\r\n\r\nabcdef";

    bool exception = false;
    size_t pos = 0;

    try {
        pos = req->Consume(raw.c_str(), raw.size());
    }
    catch (std::exception &e) {
        exception = true;
        std::cout << e.what() << std::endl;
    }

    ASSERT_EQ(exception, false);
    ASSERT_EQ(pos, raw.size());
    ASSERT_TRUE(req->IsReady() == true);
    ASSERT_EQ(req->GetVersionMajor(), 1);
    ASSERT_EQ(req->GetMethod(), "GET");
    ASSERT_EQ(req->GetUrl(), "/index.htm?name=limenghua");
    ASSERT_EQ(req->GetFullPath(), "/index.htm");
    ASSERT_EQ(req->GetHeader("Host"), "www.example.com");
    ASSERT_EQ(req->GetHeader("Content-Length"), "6");
    ASSERT_EQ(req->GetBody(), "abcdef");
}