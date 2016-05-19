#include <gtest\gtest.h>
#include <boost/any.hpp>
#include <map>
#include <memory>
#include <typeinfo>

using namespace boost;

class ServiceContainer{
public:
	template<typename ServiceType>
	void SetService(std::shared_ptr<ServiceType> service){
		auto key = typeid(ServiceType).hash_code();
		container[key] = service;
	}

	template<typename ServiceType>
	std::shared_ptr<ServiceType> GetService(){
		auto key = typeid(ServiceType).hash_code();
		auto it = container.find(key);
		if (it == container.end()){
			return std::shared_ptr<ServiceType>();
		}
		try{
			return boost::any_cast<std::shared_ptr<ServiceType>>(it->second);
		}
		catch (boost::bad_any_cast &){
			//do nothing ,后面会返回一个空的share_ptr
		}
		return std::shared_ptr<ServiceType>();
	}

private:
	typedef decltype(typeid(int).hash_code()) KeyType;
	std::map<KeyType, boost::any> container;
};


TEST(ServiceContainer, constructor)
{
	ServiceContainer anyPtrContainer;

}

TEST(ServiceContainer, SetInt)
{
	ServiceContainer serviceContainer;
	auto ptr = std::make_shared<int>(5);
	serviceContainer.SetService(ptr);
	ASSERT_EQ(serviceContainer.GetService<int>(), ptr);
	ASSERT_EQ(*(serviceContainer.GetService<int>()), 5);
}

TEST(ServiceContainer, SetString)
{
	ServiceContainer serviceContainer;
	auto ptr = std::make_shared<std::string>("hello world");
	serviceContainer.SetService(ptr);

	ASSERT_EQ(serviceContainer.GetService<std::string>(), ptr);
	ASSERT_EQ(*(serviceContainer.GetService<std::string>()), "hello world");
}

