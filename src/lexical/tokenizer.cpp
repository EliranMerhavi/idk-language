#include "pch.h"
#include "tokenizer.h"

namespace lexical
{
	tokenizer::tokenizer()
	{
	}

	tokenizer::tokenizer(const std::vector<std::string>& source_in_lines) :
		info({ 1, 0 }), source(), cursor(0), endlines(source_in_lines.size(), 0)
	{
		if (source_in_lines.size())
		{
			for (int i = 0; i < source_in_lines.size(); i++)
			{
				this->source += source_in_lines[i] + ' ';
				endlines[i] = source.size();
			}

			std::smatch match;
			if (std::regex_search(this->source, match, skip) && !eof()) // skip spaces and comments
				cursor += match.length();

			update_token_info();
		}
	}

	auto tokenizer::operator>>(token& token) -> void
	{
		if (eof())
		{
			token = { "", token_type::_EOF };
			return;
		}

		std::smatch match;
		std::string start_lexem;

		start_lexem = source.substr(cursor);

		//iterate over the token regexes
		for (const auto& [regex, type] : token_regexes)
		{
			if (std::regex_search(start_lexem, match, regex)) // token type found
			{
				token = { match[0], type, info.line, info._char };

				cursor += match.length(); // advance the cursor after the lexem

				start_lexem = source.substr(cursor);

				if (std::regex_search(start_lexem, match, skip) && !eof()) // skip spaces and comments
					cursor += match.length();

				update_token_info();
				return;
			}
		}
		// invalid lexem
		size_t start = cursor;

		while (source[cursor] != ' ' && !eof()) //skip invalid lexem
			cursor++;

		std::string invalid_lexem = start_lexem.substr(0, cursor);

		throw interpeter_error(("invalid token: \"" + invalid_lexem +
			"\" in (start: " + std::to_string(start) + ", end: " + std::to_string(cursor) + ")").c_str(), LEXICAL_ERROR);
	}

	auto tokenizer::update_token_info() -> void
	{
		if (endlines[info.line - 1] <= cursor) 
			info.line++;

		info._char = (cursor < endlines[0]) ? cursor : cursor - endlines[info.line - 2];
	}

	std::regex tokenizer::skip("^\\s+");
	std::vector<std::pair<std::regex, token_type>> tokenizer::token_regexes = {
		// keywords
		{ std::regex("^if"), token_type::IF_KEYWORD },
		{ std::regex("^else"), token_type::ELSE_KEYWORD },

		{ std::regex("^for"), token_type::FOR_KEYWORD },
		{ std::regex("^while"), token_type::WHILE_KEYWORD },
		{ std::regex("^break"), token_type::BREAK_KEYWORD},
		{ std::regex("^continue"), token_type::CONTINUE_KEYWORD},

		{ std::regex("^function"), token_type::FUNCTION_KEYWORD },
		{ std::regex("^return"), token_type::RETURN_KEYWORD },

		{ std::regex("^let"), token_type::LET_KEYWORD },
		{ std::regex("^const"), token_type::CONST_KEYWORD },

		// literals
		{ std::regex("^(true|false)"), token_type::BOOL_LITERAL },
		{ std::regex("^\\d+(\\.\\d+)"), token_type::FLOAT_LITERAL },
		{ std::regex("^\\d+"), token_type::INTEGER_LITERAL },

		{ std::regex("^\"[^\"]*\""), token_type::STRING_LITERAL },

		// operators
		{ std::regex("^->"), token_type::ARROW_OPERATOR },		
		{ std::regex("^~"), token_type::BITWISE_NEG},
		{ std::regex("^=="), token_type::EQUAL},
		{ std::regex("^!="), token_type::NEQUAL},
		{ std::regex("^&&"), token_type::LOGICAL_AND},
		{ std::regex("^\\|\\|"), token_type::LOGICAL_OR},
		{ std::regex("^!"), token_type::LOGICAL_NEG},
		{ std::regex("^<="), token_type::LESSER_EQUALS},
		{ std::regex("^>="), token_type::BIGGER_EQUALS},
		{ std::regex("^<<"), token_type::SHIFT_LEFT},
		{ std::regex("^>>"), token_type::SHIFT_RIGHT},
		{ std::regex("^="), token_type::ASSIGN},
		{ std::regex("^>"), token_type::BIGGER},
		{ std::regex("^<"), token_type::LESSER},
		{ std::regex("^\\|"), token_type::BITWISE_OR},
		{ std::regex("^&"), token_type::BITWISE_AND},
		{ std::regex("^\\+"), token_type::ADD},
		{ std::regex("^-"), token_type::SUB},
		{ std::regex("^\\*"), token_type::MULT},
		{ std::regex("^/"), token_type::DIV},
		{ std::regex("^%"), token_type::MODULO},
		{ std::regex("^\\^"), token_type::XOR},

		// punctuators (i think)
		{ std::regex("^\\["), (token_type)'[' },
		{ std::regex("^\\]"), (token_type)']' },
		{ std::regex("^\\("), (token_type)'(' },
		{ std::regex("^\\)"), (token_type)')' },
		{ std::regex("^\\{"), (token_type)'{' },
		{ std::regex("^\\}"), (token_type)'}' },
		{ std::regex("^;"), (token_type)';'	},
		{ std::regex("^:"), (token_type)':'	},
		{ std::regex("^\\,"), (token_type)',' },
		{ std::regex("^\\."), (token_type)'.' },
		// identifier
		{ std::regex("^\\w+"), token_type::IDENTIFIER },
	};
}