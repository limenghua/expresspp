#pragma once

#include <boost/asio.hpp>
#include "../expresspp/expresspp.h"
#include "../http_server/helper.h"
#include <iostream>
#include <thread>
#include <string>

using  std::cout;
using std::cin;
using std::endl;

using expresspp::HttpServer;
using expresspp::RequestPtr;
using expresspp::ResponsePtr;
using expresspp::ErrorCodePtr;
using namespace boost;


//#define HELLO_WORLD
//#define QUERY_STRING
//#define COOKIE_TEST
#define TCP_ECHO_TEST