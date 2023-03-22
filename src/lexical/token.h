#pragma once

#include <string>
#include "token_type.h"

namespace lexical
{
	/// <summary>
	/// a struct for represting a token
	/// </summary>
	struct token
	{
		std::string lexem; // the value of the token
		token_type type;   // the type of the token
		size_t line;	   // the line of the token
		size_t _char; 

		inline auto operator==(const token& other) const -> bool
		{
			return this->type == type && this->lexem == other.lexem;
		}

		/// <summary>
		/// returns true if this token is an operator token
		/// </summary>
		/// <returns></returns>
		inline auto is_operator() -> bool
		{
			return (int)type % 10 == 2;
		}
	};
}