#include<gtest\gtest.h>
#include<expresspp\request.hpp>
#include<expresspp\response.hpp>

TEST(response, defualt_constructor)
{
	expresspp::response r;

	std::string raw = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: HSmrc0sMlYUkAGmm5OPpG2HaGWk=\r\nSec-WebSocket-Protocol: chat\r\n\r\n";

	bool exception = false;
	size_t pos = 0;

	try {
		pos += r.consume(raw.c_str(), raw.size());
	}
	catch (std::exception &e) {
		exception = true;
		std::cout << e.what() << std::endl;
	}

	EXPECT_TRUE(exception == false);
	EXPECT_EQ(pos, 159);
	EXPECT_TRUE(r.headers_ready() == true);
	EXPECT_EQ(r.get_version(), "HTTP/1.1");
	EXPECT_EQ(r.get_status_code(), websocketpp::http::status_code::switching_protocols);
	EXPECT_EQ(r.get_status_msg(), "Switching Protocols");
	EXPECT_EQ(r.get_header("Upgrade"), "websocket");
	EXPECT_EQ(r.get_header("Connection"), "Upgrade");
	EXPECT_EQ(r.get_header("Sec-WebSocket-Accept"), "HSmrc0sMlYUkAGmm5OPpG2HaGWk=");
	EXPECT_EQ(r.get_header("Sec-WebSocket-Protocol"), "chat");
}

TEST(response, end)
{
	expresspp::response res;
	res.end("helloworld");

	EXPECT_TRUE(res.is_done());
	EXPECT_EQ(res.get_body(), std::string("helloworld"));
}

