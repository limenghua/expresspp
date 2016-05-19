#pragma once

#include <memory>
#include <string>

namespace expresspp {

	class ErrorCode:public std::enable_shared_from_this<ErrorCode>
	{
	protected:
		ErrorCode();
	public:
		typedef std::shared_ptr<ErrorCode> Pointer;
		typedef std::weak_ptr<ErrorCode> WeakPointer;

		static Pointer Create() {
			return Pointer(new ErrorCode());
		}
		void Reset(){
			code = 0;
			describe.clear();
		}
		/*
		 * Set New error code
		*/
		Pointer Set(int code, const std::string & describe) {
			int oldCode = this->code;
			this->code = code;
			this->describe = describe;
			return shared_from_this();
		}

		bool HasError() const{
			return code != 0;
		}

		int Code() const{
			return code;
		}
		std::string Describe()const {
			return describe;
		}

		~ErrorCode();

	private:
		int code;
		std::string describe;
	};
	typedef ErrorCode::Pointer ErrorCodePtr;

}//namespace expresspp
