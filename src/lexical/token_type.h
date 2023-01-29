#pragma once

namespace lexical
{
	enum class token_type
	{
		//maybe remove this
		TOKEN_ERROR = 0,

		//identifiers
		IDENTIFIER = -1,

		//operators
		ADD = -12,
		SUB = -22,
		MULT = -32,
		DIV = -42,
		EQUAL = -52,
		NEQUAL = -62,
		ASSIGN = -72,
		XOR = -82,
		BITWISE_OR = -92,
		BITWISE_AND = -102,
		BITWISE_NEG = -112,
		LOGICAL_OR = -122,
		LOGICAL_AND = -132,
		LOGICAL_NEG = -142,
		LESSER = -152,
		BIGGER = -162,
		LESSER_EQUALS = -172,
		BIGGER_EQUALS = -182,
		SHIFT_LEFT = -192,
		SHIFT_RIGHT = -201,
		ARROW_OPERATOR = -202,
		MODULO = -212,
		//literals
		FLOAT_LITERAL = -14,
		INTEGER_LITERAL = -24,

		STRING_LITERAL = -34,
		BOOL_LITERAL = -44,

		//maybe remove this
		CONSTANT = -5,

		//keywords
		IF_KEYWORD = -06,
		ELSE_KEYWORD = -16,

		FOR_KEYWORD = -26,
		WHILE_KEYWORD = -36,
		BREAK_KEYWORD = -46,
		CONTINUE_KEYWORD = -56,

		FUNCTION_KEYWORD = -66,
		RETURN_KEYWORD = -76,

		LET_KEYWORD = -86,
		CONST_KEYWORD = -96,

		// maybe remove these
		PUNCTUATOR = -7,
		SPEICAL_CHAR = -8,

		// eof
		_EOF = -9,
	};
}