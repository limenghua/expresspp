#include <gtest/gtest.h>
#include <boost/utility/enable_if.hpp>
#include <iostream>
#include <boost/type_traits.hpp>
#include <tuple>

class A
{
public:
	void print()
	{
		std::cout << "A::print" << std::endl;
	}
};

class B
{
public:
	void output()
	{
		std::cout << "B::output" << std::endl;
	}
};

void print(std::string fname, std::string lname, int age, bool sex){
	std::cout << "Name:" << fname << " " << lname << ":" << age << ":" << sex << std::endl;
}



#include <type_traits>
#include <utility>

template<size_t N>
struct Apply {
	template<typename F, typename T, typename... A>
	static inline auto apply(F && f, T && t, A &&... a)
		-> decltype(Apply<N - 1>::apply(
		::std::forward<F>(f), ::std::forward<T>(t),
		::std::get<N - 1>(::std::forward<T>(t)),
		::std::forward<A>(a)...
		))
	{
		return Apply<N - 1>::apply(::std::forward<F>(f),
			::std::forward<T>(t),
			::std::get<N - 1>(::std::forward<T>(t)),
			::std::forward<A>(a)...
			);
	}
};

template<>
struct Apply<0> {
	template<typename F, typename T, typename... A>
	static inline auto apply(F && f, T &&, A &&... a)
		-> decltype(::std::forward<F>(f)
		(::std::forward<A>(a)...))
	{
		return ::std::forward<F>(f)(::std::forward<A>
			(a)...);
	}
};

template<typename F, typename T>
inline auto apply(F && f, T && t)
-> decltype(Apply< ::std::tuple_size<
typename ::std::decay<T>::type
>::value>::apply(::std::forward<F>(f),
::std::forward<T>(t)))
{
	return Apply< ::std::tuple_size<
		typename ::std::decay<T>::type
	>::value>::apply(::std::forward<F>(f),
	::std::forward<T>(t));
}

auto my_test_int() -> int
{
	return 0;
}

TEST(enable_is, test)
{
	auto t = make_tuple(std::string("li"), std::string("menghua"), 32, true);
	//apply(print, t);	
}