#expresspp

#目的

最近想了解C++在Web开发中能扮演什么角色。
对于复杂的Web程序，没有Web框架支持是不可能的，不可能使用传统的CGI编写现代Web程序。
基本上，在90%的情况下，效率并不是我们首要考虑的内容。
相对于JAVA和.Net两个体系和其他几种动态语言。C++开发Web框架除了效率略高外，在其他方面毫无竞争力。
除非必须使用C++，否则不建议使用C++开发Web程序。

哪一个C++Web框架还有没有存在的必要呢？
如果有一些应用必须使用C++，或者Web并不是应用的主要内容，我们还是需要一个极轻量级，易于使用，易于扩展的C++框架。
或者有一些C/S应用，比如网络游戏，我们希望使用HTTP作为通讯协议，这样未来也可以方便的将服务器替换为其他语言。
这种情况下我们也需要一个HTTP应用，并且为了保证应用的实时性，框架应该支持Websocket协议。


#要求
1.充分使用现代C++特性。（C++11、C++14 to C++17）
2.轻量级（可以没有模板解析)
3.易于使用（充分使用C++11 lambda functian bind 等特性，简化异步和回调）
4.易于扩展（可以很方便的编写中间件）
5.支持websocket
6.接口类似Express.js

#Example

expresspp::server server;

server.use([](expresspp::request & req, expresspp::response & res){
	res<<(std::string("hello:"));
});

server.route("/hello", [](expresspp::request & req, expresspp::response & res){
	res.end(std::string("world"));
});

server.listen(9002)
	  .run();
