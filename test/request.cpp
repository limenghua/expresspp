#include "http_server\Request.h"

#include <gtest/gtest.h>

using namespace expresspp;

TEST(Request, Dict)
{
    auto req = Request::Create();

    int value;

    req->Dict().Put("name", 15);

    ASSERT_EQ(req->Dict().Get("name",0), 15);
}