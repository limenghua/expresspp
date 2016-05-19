#include "Response.h"
#include <sstream>

using namespace expresspp;

Response::Response():
	HttpParser(HTTP_RESPONSE),
	isComplated(false)
{
}


Response::~Response()
{
}


