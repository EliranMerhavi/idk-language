#pragma once
#include <string>
#include "token.h"

struct token_info
{
	size_t line;
	size_t _char;
};

namespace lexical
{
	class tokenizer
	{
	public:
		tokenizer();
		explicit tokenizer(const std::vector<std::string>& source_in_lines);

		inline auto eof() const -> bool
		{
			return cursor == source.size();
		}

		auto operator>>(lexical::token& token) -> void;
	private:
		auto update_token_info() -> void;
	private: // members
		token_info info;
		std::string source;
		std::vector<size_t> endlines;
		size_t cursor;

		static std::regex skip;
		static std::vector<std::pair<std::regex, token_type>> token_regexes;
	};
}