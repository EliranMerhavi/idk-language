#pragma once
#include <string>
#include "token.h"

namespace lexical
{
	/// <summary>
	/// the tokenizer class is responsible 
	/// for dividing the input stream into individual tokens, identifying the token type, and passing tokens one at a time to the next stage of the compiler.
	/// </summary>
	class tokenizer
	{
	/// <summary>
	/// a struct for storing info about tokens
	/// </summary>
	using token_info = struct { size_t line; size_t _char; };

	public:
		tokenizer() = default; // default constructor
		
		/// <summary>
		/// constructing a tokenizer that is
		/// ready for the lexical phase
		/// </summary>
		/// <param name="source_in_lines"></param>
		explicit tokenizer(const std::vector<std::string>& source_in_lines);

		/// <summary>
		/// </summary>
		/// <returns>return true if the tokenizer is reached for 
		/// the end of the source otherwise false</returns>
		inline auto eof() const -> bool
		{
			return cursor == source.size();
		}

		/// <summary>
		/// returns the current line
		/// </summary>
		/// <returns>the current line</returns>
		inline auto current_line() const -> size_t
		{
			return info.line;
		}

		/// <summary>
		/// used for 
		/// </summary>
		/// <param name="token"></param>
		/// <returns></returns>
		auto operator>>(lexical::token& token) -> void;
	private:
		/// <summary>
		/// updates the member info
		/// to the info for the next token
		/// </summary>
		/// <returns></returns>
		auto update_token_info() -> void;
		auto advance_cursor(size_t offset) -> void;
		/// <summary>
		/// skips white spaces and commments 
		/// </summary>
		/// <returns></returns>
		auto skip_white_spaces_and_comments() -> void;
	private: // members
		token_info info;
		std::string source;
		std::vector<size_t> endlines;
		size_t cursor;

		/// <summary>
		/// regexes for identifing different token patterns
		/// </summary>
	};
}