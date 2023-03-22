// common type definition for the project
#pragma once

#include <stdint.h>

using ldouble_t = long double;

/// <summary>
/// types of exception errors
/// </summary>
enum error_t
{
	LEXICAL_ERROR,
	SYNTAX_ERROR,
	RUNTIME_ERROR
};

inline auto error_t_to_string(error_t type) -> std::string
{
	std::string res;
	switch (type)
	{
	case LEXICAL_ERROR:
		res = "LEXICAL_ERROR";
		break;
	case SYNTAX_ERROR:
		res = "SYNTAX_ERROR";
		break;
	case RUNTIME_ERROR:
		res = "RUNTIME_ERROR";
		break;
	}

	return res;
}

/// <summary>
/// struct that represenst all types of interpeter error
/// </summary>
struct interpeter_error
{
	std::string msg;
	error_t type;
	interpeter_error(const std::string& msg, error_t type);
};

#define interpeter_assert(condition, message, error_type) if (!(condition)) throw interpeter_error{ message, error_type }
#define assert(condition, message) if (!(condition)) throw std::exception(message)