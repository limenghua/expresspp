#pragma once

#include<memory>
#include<string>
#include<map>
#include "http_parser/http_parser.h"
#include "http_server/utils.h"

namespace expresspp{

	class HttpParser;
	inline HttpParser * ContainerOf(http_parser* p_) {
		return static_cast<HttpParser*>(p_->data);
	}

	#define HTTP_CB(name)                                                         \
	  static int name(http_parser* p_) {                                          \
		HttpParser* self = ContainerOf(p_);										  \
		return self->name##_();                                                   \
	  }                                                                           \
	  int name##_()


	#define HTTP_DATA_CB(name)                                                    \
	  static int name(http_parser* p_, const char* at, size_t length) {           \
		HttpParser* self = ContainerOf( p_);					                  \
		return self->name##_(at, length);                                         \
	  }                                                                           \
	  int name##_(const char* at, size_t length)

/// Base HTTP parser
/**
* Includes methods and data elements common to all types of HTTP messages such
* as headers, versions, bodies, etc.
*/
class HttpParser //: public std::enable_shared_from_this<HttpParser>
{
public:
	typedef std::shared_ptr<HttpParser> Pointer;
	typedef std::weak_ptr<HttpParser> WeakPointer;
	const size_t MAX_BODY_SIZE = 1024 * 1024;

protected:
	HttpParser(enum http_parser_type type);
public:
	~HttpParser();

	static Pointer Create(enum http_parser_type type) {
		return Pointer(new HttpParser(type));
	}
	void Initialize(enum http_parser_type type);

	HTTP_CB(on_message_begin);
	HTTP_DATA_CB(on_url);
	HTTP_DATA_CB(on_status);
	HTTP_DATA_CB(on_header_field);
	HTTP_DATA_CB(on_header_value);
	HTTP_CB(on_headers_complete);
	HTTP_DATA_CB(on_body);
	HTTP_CB(on_message_complete);

	/// Process bytes in the input buffer
	/**
	* Process up to len bytes from input buffer buf. Returns the number of
	* bytes processed. Bytes left unprocessed means bytes left over after the
	* final header delimiters.
	*
	* Consume is a streaming processor. It may be called multiple times on one
	* request and the full headers need not be available before processing can
	* begin. If the end of the request was reached during this call to consume
	* the ready flag will be set. Further calls to consume once ready will be
	* ignored.
	*
	* Consume will throw an http::exception in the case of an error. Typical
	* error reasons include malformed requests, incomplete requests, and max
	* header size being reached.
	*
	* @param buf Pointer to byte buffer
	* @param len Size of byte buffer
	* @return Number of bytes processed.
	*/
	size_t Consume(char const * buf, size_t len);

	/// Returns whether or not the request is ready for reading.
	bool IsReady() const {
		return isReady;
	}

	/// Get the value of an HTTP header
	/**
	* @todo Make this method case insensitive.
	*
	* @param [in] key The name/key of the header to get.
	* @return The value associated with the given HTTP header key.
	*/
	std::string const & GetHeader(std::string const & key) const;


	/// Append a value to an existing HTTP header
	/*
	* @param [in] key The name/key of the header to append to.
	* @param [in] val The value to append.
	*/
	void SetHeader(std::string const & key, std::string const & val) {
		headers[key] = val;
	}

	/// Remove a header from the parser
	/**
	* @param [in] key The name/key of the header to remove.
	*/
	void RemoveHeader(std::string const & key) {
		headers.erase(key);
	}

    /// 获取Http 头设置完成后的字符串
    std::string  GetHeaderString();

	/// Get HTTP body
	/**
	* Gets the body of the HTTP object
	*
	* @return The body of the HTTP message.
	*/
	std::string const & GetBody() const {
		return body;
	}

	/// Set body content
	/**
	* Set the body content of the HTTP response to the parameter string. Note
	* set_body will also set the Content-Length HTTP header to the appropriate
	* value. If you want the Content-Length header to be something else, do so
	* via replace_header("Content-Length") after calling set_body()
	*
	* @param value String data to include as the body content.
	*/
	void SetBody(const  std::string & value);

	const std::string & GetUrl()const {
		return url;
	}

    void SetUrl(const std::string & value){
        url = value;
    }

    std::string GetFullPath()const{
        auto pos = url.find('?');
        if (pos == url.npos){
            return url;
        }
        return url.substr(0, pos);
    }

	const std::string & GetMethod()const {
		return method;
	}
    void SetMethod(const std::string & value){
        method = value;
    }

    unsigned int GetStatusCode()const {
		return status_code;
	}
    void SetStatusCode(unsigned int code) {
        status_code = code;
    }

    unsigned short GetVersionMajor()const {
        return versionMajor;
    }
    void SetVersionMajor(unsigned short value){
        versionMajor = value;
    }
    unsigned short GetVersionMinor()const{
        return versionMinor;
    }
    void SetVersionMinor(unsigned short value){
        versionMinor = value;
    }

	bool ShouldKeepAlive() {
		return http_should_keep_alive(&parser)!=0;
	}
	bool IsUpgrade() {
		parser.upgrade;
	}

	std::map<std::string, std::string> & GetHeaders(){
		return headers;
	}

private:

	bool			isReady;

    unsigned short  versionMajor;
    unsigned short  versionMinor;

	std::string     method;
	std::string     url;
    unsigned int	status_code;
	std::string     body;

	std::string		lastHeaderField_;
	std::map<std::string,std::string> headers;

	http_parser		parser;
	static const struct http_parser_settings settings;
};


typedef HttpParser::Pointer HttpParserPtr;

}; //namespace expresspp
