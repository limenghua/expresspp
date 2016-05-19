#include <gtest\gtest.h>

#include <iostream>
#include <string>
#include <regex>

TEST(Regex, email)
{
	std::string phone_pantern =
		"(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ]?)(\\d{4})";

	//example
	//"/hello/::uer/::name/

	std::string url_pantern =
		"(::)([[:alpha:]]*)(/)";

	std::regex r(url_pantern);

	std::smatch m;
	std::string s;

	std::cout << "ÇëÊäÈëµç»°ºÅÂë:" << std::endl;
	while (getline(std::cin, s)){
		if (s.empty())break;

		for (std::sregex_iterator it(s.begin(), s.end(), r), end_it; it != end_it; it++){
			std::cout << it->str(2) << std::endl;
		}
		if (std::regex_search(s, m, r)){
			//std::cout << m.str(0) << "-" << m.str(1) << "-" << m.str(2) << std::endl;
		}
	}

}