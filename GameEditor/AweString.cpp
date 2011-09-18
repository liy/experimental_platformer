#include "AweString.h"

#include <Awesomium/awesomium_capi.h>

std::string AweString::std_str(const awe_string* awe_str){
	int str_size = awe_string_to_utf8(awe_str, NULL, 0) + 1;
	char* str_char = new char[str_size];
	awe_string_to_utf8(awe_str, str_char, str_size);

	return std::string(str_char);
}

AweString::AweString(const std::string& str)
{
	_awe_str = awe_string_create_from_ascii(str.c_str(), str.length());
}

AweString::AweString(const char* chars)
{
	_awe_str = awe_string_create_from_ascii(chars, strlen(chars));
}


AweString::~AweString(void)
{
	awe_string_destroy(_awe_str);
}

const awe_string* AweString::awe_str() const
{
	return _awe_str;
}