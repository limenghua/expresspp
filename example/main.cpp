#include "example\config.h"

int main()
{
#ifdef HELLO_WORLD
    void ExampleHelloWorld();
    ExampleHelloWorld();
#endif //HELLO_WORLD

#ifdef QUERY_STRING
    void ExampleQueryString();
    ExampleQueryString();
#endif
  
#ifdef COOKIE_TEST
    void ExampleCookie();
    ExampleCookie();
#endif

#ifdef TCP_ECHO_TEST
	void TcpEcho();
	TcpEcho();
#endif

    return 0;
}