#include <gtest\gtest.h>
#include <http_server\utils.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

using namespace expresspp;

TEST(FileIo, StringStream)
{
    std::ifstream fin;
    fin.open("http_parser.cpp");

	if (!fin){
		std::cout << "conn't open file" << std::endl;
	}

    std::stringstream s;
	std::string str;
	char buf[1024];
	size_t bytes = 0;


    std::cout << s.str();
}
