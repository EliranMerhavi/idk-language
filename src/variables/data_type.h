#pragma once

#include <string>

/// <summary>
/// list for all the data types
/// that the language supports
/// </summary>
enum class data_type
{
	INTEGER,
	FLOAT,
	STRING,
	OBJECT,
	BOOL,
	ARRAY,
	FUNCTION,
	VOID
};

inline auto data_type_to_string(data_type type) -> std::string
{
	std::string str;

	switch (type)
	{
	case data_type::INTEGER:
		str = "int";
		break;
	case data_type::FLOAT:
		str = "float";
		break;
	case data_type::STRING:
		str = "string";
		break;
	case data_type::OBJECT:
		str = "object";
		break;
	case data_type::BOOL:
		str = "bool";
		break;
	case data_type::ARRAY:
		str = "array";
		break;
	case data_type::VOID:
		str = "void";
		break;
	case data_type::FUNCTION:
		str = "func_type";
		break;
	default:
		throw std::exception("unknown type");
		break;
	}

	return str;
}

inline auto data_type_from_string(const std::string& str) -> data_type
{
    data_type type = (data_type)-1;

	if (str == "void")
		type = data_type::VOID;
	else if (str == "object")
		type = data_type::OBJECT;
	else if (str == "array")
		type = data_type::ARRAY;
	else if (str == "float")
		type = data_type::FLOAT;
	else if (str == "int")
		type = data_type::INTEGER;
    else if (str == "string")
        type = data_type::STRING;
    else if (str == "bool")
        type = data_type::BOOL;
    else if (str == "func_type")
        type = data_type::FUNCTION;

    return type;
}