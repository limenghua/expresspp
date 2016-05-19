#include "expresspp\function_traits.hpp"
#include <gtest\gtest.h>

auto fun(int a, double b)
->decltype(a+b)
{
	return a + b;
}



using namespace expresspp;
TEST(FunctionTraits, fun)
{
	function_traits<decltype(fun)>::tuple_type tuple;

	std::get<0>(tuple) = 5;
	std::get<1>(tuple) = 6;

	ASSERT_EQ(apply(fun, tuple),11);
}