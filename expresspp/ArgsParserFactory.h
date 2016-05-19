#pragma once

#include "IArgsParser.h"
#include <vector>

namespace expresspp{

	class ParmsArgsParser :public ArgsParser
	{
	public:
		ParmsArgsParser(const std::string & pattern);
		virtual void Parse( RequestPtr req)override;
		virtual bool Match(const std::string & pattern)override;
		virtual std::string GetRoute() override;

		static Pointer Create(const std::string & pattern){
			return Pointer(new ParmsArgsParser(pattern));
		}
	private:
		void ParseParmsName();

	private:
		std::vector<std::string> parmsName;

	};


	class ArgsParserFactory
	{
	public:
		ArgsParserFactory();
		~ArgsParserFactory();

		static ArgsParserPtr CreateParser(const std::string & pattern){
			return ParmsArgsParser::Create(pattern);
		}
	};

};//namespace expresspp{

