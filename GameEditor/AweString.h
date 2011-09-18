#pragma once

#include <iostream>

typedef struct _awe_string awe_string;

/**
 * A helper class for awe_string creation and destroy. It also be able to create a standard string from awe_string using static function.
 */
class AweString
{
public:
	AweString(const std::string& str);
	AweString(const char* chars);
	~AweString(void);

	/**
	 * Static helper function to create a standard string
	 */
	static std::string std_str(const awe_string* awe_str);

	/**
	 * Get the awe_string
	 */
	const awe_string* awe_str() const;

protected:
	/**
	 * Converted awe_string
	 */
	awe_string* _awe_str;
};

