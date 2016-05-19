#pragma once

#include <boost/any.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <map>
#include <time.h>
#include <memory>
#include <string>
#include <sstream>
#include <typeinfo>

namespace expresspp{

	/// 存储任意类型shared_ptr对象的容器
	/**
	* 使用boosy.any类型容纳任意类型的shared_ptr对象
	* 使用std::string作为键进行插入，查找和删除操作
	*/
    class PTree{
    public:
        template<typename Type>
        void Put(const std::string &key, const Type & defualtValue){
            ptree.put(key, defualtValue);
        }

        template<typename Type>
        Type Get(const std::string &key, const Type & defualtValue){
            return ptree.get(key,defualtValue);
        }

        template<typename Type>
        Type Get(const std::string & key){
            return ptree.get<Type>(key);
        }

        void Delete(const std::string &key){
            ptree.erase(key);
        }
        void DeleteAll(){
            ptree.clear();
        }

        std::string ToJson(){
            std::stringstream ss;
            boost::property_tree::json_parser::write_json(ss, ptree);
            return ss.str();
        }

        std::string ToXml(){
            std::stringstream ss;
            boost::property_tree::xml_parser::write_xml(ss, ptree);
            return ss.str();
        }

    private:
        boost::property_tree::ptree ptree;
    };


	class AnyPtrContainer{
	public:
		template<typename ServiceType>
		void Put(const std::string &key, std::shared_ptr<ServiceType> ptr){
			container[key] = ptr;
		}
        template<typename ServiceType>
        void Put(std::shared_ptr<ServiceType> ptr){
            std::string key = typeid(ServiceType).raw_name();
            Put<ServiceType>(key, ptr);
        }

        template<typename ServiceType>
        void Put(const std::string &key, std::weak_ptr<ServiceType> ptr){
            container[key] = ptr;
        }
        template<typename ServiceType>
        void Put(std::weak_ptr<ServiceType> ptr){
            std::string key = typeid(ServiceType).raw_name();
            Put<ServiceType>(key, ptr);
        }

		template<typename ServiceType>
		std::shared_ptr<ServiceType> Get(const std::string &key){
			std::shared_ptr<ServiceType> result;

			auto it = container.find(key);
			if (it != container.end()){
                auto any = it->second;
                try{
                    if (any.type() == typeid(std::shared_ptr<ServiceType>)){
                        result = boost::any_cast<std::shared_ptr<ServiceType>>(any);
                    }
                    else if (any.type() == typeid(std::weak_ptr<ServiceType>)){
                        auto weak = boost::any_cast<std::weak_ptr<ServiceType>>(any);
                        result = weak.lock();
                    }
                }
				catch (boost::bad_any_cast &){
					//do nothing ,后面会返回一个空的share_ptr
				}
			}
			return result;
		}
        template<typename ServiceType>
        std::shared_ptr<ServiceType> Get(){
            std::string key = typeid(ServiceType).raw_name();
            return Get<ServiceType>(key);
        }
		void Delete(const std::string &key){
			container.erase(key);
		}
        template<typename ServiceType>
        void Delete(){
            std::string key = typeid(ServiceType).raw_name();
            return Delete(key);
        }

        void DeleteAll(){
            container.clear();
        }

	private:
		std::map<std::string, boost::any> container;
	};
}

