#pragma once

#include <boost/asio.hpp>
#include "HttpParser.h"
#include "utils.h"
#include "AnyContainer.h"
#include <memory>
#include <sstream>

namespace expresspp {
	
	class Response: public HttpParser
	{
	protected:
		Response();
	public:
		typedef std::shared_ptr<Response> Pointer;
		typedef std::weak_ptr<Response> WeakPointer;
		typedef boost::asio::streambuf BufferType;

		static Pointer Create() {
			return Pointer(new Response());
		}

		void Reset(){
            isComplated = false;
			
            Initialize(HTTP_RESPONSE);
		}
		
		void Write(const char * buf, size_t len) {
			buffer.sputn(buf, len);
		}
		void Write(const std::string & str) {
			Write(str.c_str(),str.length());
		}

		template <typename T>
		Response & operator <<(T && value){
			std::stringstream ss;
			ss << (std::forward<T>(value));
			Write(ss.str());
			return *this;
		}

		bool IsComplated(){
			return isComplated;
		}

		void End() {
			assert(!isComplated);
			if (isComplated)
			{
				throw std::exception("End Cann't call more than one times");
			}
			size_t bodyLenth = buffer.size();

			if (bodyLenth == 0) {
				RemoveHeader("Content-Length");
			}
			else{
				std::stringstream len;
				len << bodyLenth;
				SetHeader("Content-Length", len.str());
			}

			isComplated = true;
		}

		void End(const char * buf, size_t len) {
			Write(buf, len);
			End();
		}

		void End(const std::string & msg) {
			Write(msg);
			End();
		}

		boost::asio::streambuf & GetBuffer(){
			return buffer;
		}
		
		~Response();

        PTree & Dict(){
            return dict;
        }

        template<typename ServiceType>
        void Set(const std::string &key, std::shared_ptr<ServiceType> ptr){
            attrs.Put(key, ptr);
        }
        template<typename ServiceType>
        void Set(std::shared_ptr<ServiceType> ptr){
            attrs.Put<ServiceType>(ptr);
        }

        template<typename ServiceType>
        std::shared_ptr<ServiceType> Get(const std::string &key){
            return attrs.Get<ServiceType>(key);
        }
        template<typename ServiceType>
        std::shared_ptr<ServiceType> Get(){
            return attrs.Get<ServiceType>();
        }

    private:
		//Ω˚÷π∏∏¿‡SetBody∑Ω∑®
		void SetBody(const std::string &);

        AnyPtrContainer attrs;
        PTree  dict;

		boost::asio::streambuf buffer;
		bool	isComplated;
	};
	typedef Response::Pointer ResponsePtr;
	typedef Response::WeakPointer ResponseWeakPtr;

}//expresspp
