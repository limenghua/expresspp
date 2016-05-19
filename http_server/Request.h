#pragma once

#include <memory>
#include "AnyContainer.h"
#include "HttpParser.h"

namespace expresspp {

	class Request : public HttpParser
	{
	protected:
		Request();
	public:
		typedef std::shared_ptr<Request> Pointer;
		typedef std::weak_ptr<Request> WeakPointer;

        static Pointer Create() {
            return Pointer(new Request());
		}

		void Reset(){
            settings.DeleteAll();
            dict.DeleteAll();
            Initialize(HTTP_REQUEST);
		}

        PTree & Dict(){
            return dict;
        }

		template<typename ServiceType>
		void Set(const std::string &key, std::shared_ptr<ServiceType> ptr){
			settings.Put(key,ptr);
		}
        template<typename ServiceType>
        void Set(std::shared_ptr<ServiceType> ptr){
            settings.Put<ServiceType>(ptr);
        }

		template<typename ServiceType>
		std::shared_ptr<ServiceType> Get(const std::string &key){
            return settings.Get<ServiceType>(key);
		}
        template<typename ServiceType>
        std::shared_ptr<ServiceType> Get(){
            return settings.Get<ServiceType>();
        }

	private:
		AnyPtrContainer settings;
        PTree  dict;
	};

	typedef Request::Pointer RequestPtr;
	typedef Request::WeakPointer RequestWeakPtr;

}//namespace expresspp 

