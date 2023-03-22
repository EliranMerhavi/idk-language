#pragma once

#include <string>

/// <summary>
/// the variable types
/// </summary>
enum class var_type
{
	LET, // mutable
	CONST // immutable
};

inline auto var_type_from_string(const std::string& str) -> var_type
{
    var_type type = (var_type)-1;

    if (str == "let")
        type = var_type::LET;
    else if (str == "const")
        type = var_type::CONST;

    if (type == (var_type)-1)
        throw std::exception("unknown variable type");

    return type;
}
