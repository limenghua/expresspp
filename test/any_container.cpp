#include "http_server\AnyContainer.h"
#include <gtest\gtest.h>

using namespace boost;
using namespace expresspp;

TEST(AnyPtrContainer, constructor)
{
	AnyPtrContainer anyPtrContainer;
}

TEST(AnyPtrContainer, PutShared)
{
	AnyPtrContainer anyPtrContainer;
	auto ptr = std::make_shared<int>(5);
	anyPtrContainer.Put("age",ptr);

    ASSERT_EQ(ptr.use_count(), 2);
	ASSERT_EQ(anyPtrContainer.Get<int>("age"), ptr);
	ASSERT_EQ(*(anyPtrContainer.Get<int>("age")),5);
}

TEST(AnyPtrContainer, PutNoShared)
{
    AnyPtrContainer anyPtrContainer;

    int a;
    std::string b;
    double c;
    class AnyOtherTypeNoSharedPtr{

    };
    AnyOtherTypeNoSharedPtr someType;
    
    //anyPtrContainer.Put("a", a);    //无法通过编译
    anyPtrContainer.Put("a", std::make_shared<int>(a));
    //anyPtrContainer.Put("b", b);    //无法通过编译
    anyPtrContainer.Put("b", std::make_shared<std::string>(b));
    //anyPtrContainer.Put("c", c);   //无法通过编译
    anyPtrContainer.Put("b", std::make_shared<double>(c));
    //anyPtrContainer.Put("someType", someType);   //无法通过编译
    anyPtrContainer.Put("someType", std::make_shared<AnyOtherTypeNoSharedPtr>());
}

TEST(AnyPtrContainer, WeakPtr)
{
    AnyPtrContainer anyPtrContainer;
    auto ptr = std::make_shared<int>(5);
    auto weakPtr = std::weak_ptr<int>(ptr);
    anyPtrContainer.Put("age", weakPtr);

    ASSERT_EQ(ptr.use_count(), 1);
    ASSERT_EQ(anyPtrContainer.Get<int>("age"), ptr);
    ASSERT_EQ(*(anyPtrContainer.Get<int>("age")), 5);
}


TEST(AnyPtrContainer, Get)
{
	AnyPtrContainer anyPtrContainer;
	auto ptr = std::make_shared<std::string>("hello world");
	anyPtrContainer.Put("hello",ptr);

	ASSERT_EQ(anyPtrContainer.Get<std::string>("hello"), ptr);
	ASSERT_EQ(*(anyPtrContainer.Get<std::string>("hello")), "hello world");
}

TEST(AnyPtrContainer, Delete)
{
	AnyPtrContainer anyPtrContainer;
	auto ptr = std::make_shared<std::string>("hello world");
	anyPtrContainer.Put("hello", ptr);

	ASSERT_EQ(anyPtrContainer.Get<std::string>("hello"), ptr);
	ASSERT_EQ(*(anyPtrContainer.Get<std::string>("hello")), "hello world");

	anyPtrContainer.Delete("hello");
	ptr = anyPtrContainer.Get<std::string>("hello");
	ASSERT_FALSE(ptr);
}


TEST(PTree, Put)
{
    PTree pt;
    pt.Put("name", std::string("li"));
    pt.Put("age", 15);

    ASSERT_EQ(pt.Get("name", std::string()), std::string("li"));
    ASSERT_EQ(pt.Get("age", 10), 15);
    ASSERT_NO_THROW(pt.Get("unkowan", 10));
    ASSERT_EQ(pt.Get("unkowan",10), 10);
}

TEST(PTree, Get)
{
    PTree pt;
    pt.Put("name", std::string("li"));
    pt.Put("age", 15);
    pt.Put("Char", 'A');

    ASSERT_EQ(pt.Get<char>("Char"), 'A');
    ASSERT_EQ(pt.Get("age", 10), 15);
    ASSERT_ANY_THROW(pt.Get<int>("Char"));
}

TEST(PTree, ToJson)
{
    PTree pt;
    pt.Put("name", std::string("li"));
    pt.Put("age", 15);

    std::cout << pt.ToJson() << std::endl;
}

TEST(PTree, ToXml)
{
    PTree pt;
    pt.Put("name", std::string("li"));
    pt.Put("age", 15);
    pt.Put("Sex", true);

    std::cout << pt.ToXml() << std::endl;
}