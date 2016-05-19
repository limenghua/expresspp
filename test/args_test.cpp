#include <gtest\gtest.h>
#include <http_server\utils.h>

using namespace expresspp;

TEST(Util, StrSplit)
{
    std::string str = "name=li&age=6&sex=true";
    auto result = StringSplit(str, '&');
    ASSERT_EQ(result.size(),3);
    ASSERT_EQ(result[0], "name=li");
    ASSERT_EQ(result[2], "sex=true");
}

TEST(Util, ParseQueryString)
{
    std::string str = "name=li&age=6&sex=true";
    auto result = ParseQueryString(str);

    ASSERT_EQ(typeid(result) , typeid(std::map<std::string, std::string>));

    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result["name"], "li");
    ASSERT_EQ(result["age"], "6");
    ASSERT_EQ(result["sex"], "true");
}

