#define _WEBSOCKETPP_CPP11_THREAD_
#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#define _WEBSOCKETPP_CPP11_SYSTEM_ERROR_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_MEMORY_
#define _SCL_SECURE_NO_WARNINGS

#include<expresspp\expresspp.hpp>


int main() {
    // Create a server endpoint
	expresspp::server server;

    try {
        // Set logging settings

		server.use([](expresspp::request & req, expresspp::response & res){
			res<<(std::string("hello"));
		});

		server.route("/hello", [](expresspp::request & req, expresspp::response & res){
			res.end(std::string("world"));
		});

        // Listen on port 9002
		server.listen(9002).run();

    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
