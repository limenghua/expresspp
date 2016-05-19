#include <gtest\gtest.h>
#include <iostream>

TEST(Lambda, Parms)
{
	std::string s("hello");
	std::string & s2 = s;
	std::string & s3 = s;
	std::string & s4 = s;
	[&s2, &s3,s4](){
		ASSERT_EQ(&s2, &s3);
		ASSERT_NE(&s2, &s4);
	}();
	//说明:lambda表达式中,对引用类型变量,传入lambda表达式后默认为值拷贝.
}