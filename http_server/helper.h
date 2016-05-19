#pragma once

#include "Response.h"

namespace expresspp{

	inline void Redirect(ResponsePtr res, const std::string& url)
	{
		res->SetStatusCode(302);
		res->SetHeader("Location", url);
		//res->SetBody("");
		res->End("<p>Moved Temporarily. Redirecting to <a href=\"" + url + "\">/</a></p>");
	}




} //namespace expresspp{

