#include "HttpParser.h"
#include "utils.h"

#include <sstream>
#include <string>

using namespace expresspp;

const struct http_parser_settings HttpParser::settings = {
	HttpParser::on_message_begin,
	HttpParser::on_url,
	HttpParser::on_status,
	HttpParser::on_header_field,
	HttpParser::on_header_value,
	HttpParser::on_headers_complete,
	HttpParser::on_body,
	HttpParser::on_message_complete,
	nullptr,  // on_chunk_header
	nullptr   // on_chunk_complete
};

HttpParser::HttpParser(enum http_parser_type type) 
{
	Initialize(type);
}
void HttpParser::Initialize(enum http_parser_type type) {
	http_parser_init(&parser, type);
	parser.data = (void *)(this);

    versionMajor = 1;
    versionMinor = 1;

	method.clear();
	url.clear();
	body.clear();
	headers.clear();

	lastHeaderField_.clear();
	isReady = false;
}

size_t HttpParser::Consume(char const * buf, size_t len)
{
	size_t bytes = http_parser_execute(&parser, &settings, buf, len);
	if (bytes != len && parser.http_errno) {
		throw std::exception(http_errno_name((enum http_errno)parser.http_errno), parser.http_errno);
	}
	return bytes;
}


HttpParser::~HttpParser()
{
}

const std::string EMPTY_STR;
std::string const & HttpParser::GetHeader(std::string const & key) const
{
	auto it = headers.find(key);
	if (it == headers.end()) {
		return EMPTY_STR;
	}
	return it->second;
}

void HttpParser::SetBody(const std::string  & value) {
	if (value.size() == 0) {
		RemoveHeader("Content-Length");
		body = "";
		return;
	}

	// TODO: should this method respect the max size? If so how should errors
	// be indicated?

	std::stringstream len;
	len << value.size();
	SetHeader("Content-Length", len.str());
	body = value;

}

std::string HttpParser::GetHeaderString()
{
    std::stringstream  headerStream;
    headerStream << "HTTP/";
    headerStream << versionMajor << "." << versionMinor;

    headerStream << " ";
    headerStream << GetStatusCode();
    headerStream << " ";
    headerStream << StatusMessage(GetStatusCode());
    headerStream << "\r\n";

    headerStream << "Date: ";
    headerStream << HeaderDateStr();
    headerStream << "\r\n";

    for (auto item : GetHeaders()) {
        headerStream << item.first;
        headerStream << ":";
        headerStream << item.second;
        headerStream << "\r\n";
    }

    headerStream << "\r\n";
    return headerStream.str();
}


int HttpParser::on_message_begin_()
{
	url.clear();
	isReady = false;
	return 0;
}

int HttpParser::on_url_(const char* at, size_t length)
{
	url = std::string(at, length);
	return 0;
}

int HttpParser::on_status_(const char* at, size_t length)
{
	return 0;
}
int HttpParser::on_header_field_(const char* at, size_t length)
{
	if (lastHeaderField_.empty()) {
		lastHeaderField_ = std::string(at, length);
	}

	return 0;
}

int HttpParser::on_header_value_(const char* at, size_t length)
{
	std::string headerValue(at, length);
	if (!lastHeaderField_.empty()) {
		SetHeader(lastHeaderField_, headerValue);
		lastHeaderField_.clear();
	}

	return 0;
}

int HttpParser::on_headers_complete_()
{
	if (parser.type == HTTP_REQUEST) {
		method = std::string(http_method_str((http_method)parser.method));
        SetVersionMajor(parser.http_major);
        SetVersionMinor(parser.http_minor);
	}

    if (parser.type == HTTP_RESPONSE){
        SetStatusCode(parser.status_code);
    }

	return 0;
}

int HttpParser::on_body_(const char* at, size_t length)
{
	if (body.length() + length < MAX_BODY_SIZE) {
		body += std::string(at, length);
	}
	return 0;
}

int HttpParser::on_message_complete_()
{
	isReady = true;

	return 0;
}