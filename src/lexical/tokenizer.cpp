#include "pch.h"
#include "tokenizer.h"

namespace lexical
{
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

			skip_white_spaces_and_comments();
		}
	}

	auto tokenizer::operator>>(token& token) -> void
	{
		static std::vector<std::pair<std::regex, token_type>> token_regexes = {
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

		// -----------------------------------------------------------
		//	START OF CODE
		// -----------------------------------------------------------

		if (eof())
		{
			token = lexical::token{ "", token_type::_EOF, info.line - 1, info._char };
			return;
		}

		std::smatch lexem_holder;
		std::string start_lexem;

		start_lexem = source.substr(cursor);

		//iterate over the token regexes
		for (const auto& [regex, type] : token_regexes)
		{
			if (std::regex_search(start_lexem, lexem_holder, regex)) // token type found
			{
				token = lexical::token{ lexem_holder[0], type, info.line, info._char };
				advance_cursor(lexem_holder.length());
				skip_white_spaces_and_comments();
				return;
			}
		}
		// invalid lexem
		size_t start = cursor;

		while (source[cursor] != ' ' && !eof()) //get invalid lexem length
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

	auto tokenizer::advance_cursor(size_t offset) -> void
	{
		cursor += offset;
		update_token_info();
	}

	auto tokenizer::skip_white_spaces_and_comments() -> void
	{
		bool skip_flag = true;
	
		while (!eof() && skip_flag)
		{
			char ch = source[cursor];
			switch (ch)
			{
			case '?': // one line comment
				if (cursor + 1 >= source.size() || source[cursor + 1] != '?')
				{
					skip_flag = false; // we hitted an identifier ?
				}
				else
				{
					size_t curr_line = current_line();
					while (!eof() && curr_line == current_line())
					{
						advance_cursor(1);
					}
				}
				break;
			case '#':
			{
				interpeter_assert(cursor != source.size() && source[cursor + 1] == '#', "missing end of multiline comment", LEXICAL_ERROR);

				advance_cursor(2);
				bool in_comment = true;
				
				while (in_comment)
				{
					char ch1 = source[cursor];

					advance_cursor(1);

					interpeter_assert(!eof(), "missing end of multiline comment", LEXICAL_ERROR);

					char ch2 = source[cursor];

					if (ch1 == '#' && ch2 == '#') 
					{ 
						advance_cursor(1);
						in_comment = false;
					}
				}
				break;
			}
			case ' ':
			case '\t':
				advance_cursor(1);
				break;
			default:
				skip_flag = false;
				break;
			}

		}
	}

	
	
}