#pragma once
#include <functional>
#include <tuple>

namespace expresspp{

	//普通函数.
	//函数指针.
	//function/lambda
	//成员函数.
	//函数对象.

	//转换为std::function和函数指针.
	template<typename T>
	struct function_traits;

	//普通函数.
	template<typename Ret, typename... Args>
	struct function_traits < Ret(Args...) >
	{
	public:
		enum { arity = sizeof...(Args) };
		typedef Ret function_type(Args...);
		typedef Ret return_type;
		using stl_function_type = std::function < function_type > ;
		typedef Ret(*pointer)(Args...);
		typedef std::tuple<Args...> tuple_type;

		template<size_t I>
		struct args
		{
			static_assert(I < arity, "index is out of range, index must less than sizeof Args");
			using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
		};
	};

	//函数指针.
	template<typename Ret, typename... Args>
	struct function_traits<Ret(*)(Args...)> : function_traits < Ret(Args...) > {};

	//std::function
	template <typename Ret, typename... Args>
	struct function_traits<std::function<Ret(Args...)>> : function_traits < Ret(Args...) > {};

	//member function
#define FUNCTION_TRAITS(...) \
	template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{}; \

	FUNCTION_TRAITS()
		FUNCTION_TRAITS(const)
		FUNCTION_TRAITS(volatile)
		FUNCTION_TRAITS(const volatile)

		//函数对象.
		template<typename Callable>
	struct function_traits : function_traits < decltype(&Callable::operator()) > {};

	template <typename Function>
	typename function_traits<Function>::stl_function_type to_function(const Function& lambda)
	{
		return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
	}

	template <typename Function>
	typename function_traits<Function>::stl_function_type to_function(Function&& lambda)
	{
		return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
	}

	template <typename Function>
	typename function_traits<Function>::pointer to_function_pointer(const Function& lambda)
	{
		return static_cast<typename function_traits<Function>::pointer>(lambda);
	}



	template<size_t N>
	struct Apply{
		template<typename F, typename T, typename... A>
		static inline auto apply(F &&f, T && t, A &&... a)
			->decltype(Apply<N - 1>::apply(
			std::forward<F>(f),
			std::forward<T>(t),
			std::get<N - 1>(std::forward<T>(t)),
			std::forward<A>(a)...
			))
		{
			return
				(Apply<N - 1>::apply(
				std::forward<F>(f),
				std::forward<T>(t),
				std::get<N - 1>(std::forward<T>(t)),
				std::forward<A>(a)...
				));
		}
	};

	template<>
	struct Apply < 0 > {
		template<typename F, typename T, typename... A>
		static inline auto apply(F && f, T && t, A &&... a)
			->decltype(
			std::forward<F>(f)(std::forward<A>(a)...)
			)
		{
			return std::forward<F>(f)(std::forward<A>(a)...);
		}
	};

	template<typename F, typename T>
	inline auto apply(F && f, T && t)
		->decltype(
		Apply<std::tuple_size<typename std::decay<T>::type>::value>::apply(
		std::forward<F>(f), std::forward<T>(t)))
	{
		return Apply<std::tuple_size<typename std::decay<T>::type>::value>::apply(
			std::forward<F>(f), std::forward<T>(t));
	}

};//namespace expresspp{