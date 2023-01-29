#pragma once

#include <string>
#include "token_type.h"

namespace lexical
{
	struct token
	{
		std::string lexem;
		token_type type;
		size_t line;
		size_t _char;

		inline auto operator==(const token& other) const -> bool
		{
			return this->type == type && this->lexem == other.lexem;
		}

		inline auto is_operator() -> bool
		{
			return (int)type % 10 == 2;
		}
	};
}