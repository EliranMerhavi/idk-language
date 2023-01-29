#pragma once

#include "typedefs.h"
#include "variables/data_type.h"
#include "variables/var_type.h"
#include "lexical/token_type.h"

using namespace std;

namespace ast
{
	enum class expr_type
	{
		PROGRAM_EXPR,
		FUNCTION_DECLARATION_EXPR,
		VARIABLE_DECLARATION_EXPR,
		BLOCK_EXPR,
		IF_EXPR,
		WHILE_EXPR,
		FOR_EXPR,
		RETURN_EXPR,
		BREAK_EXPR,
		CONTINUE_EXPR,
		BINARY_EXPR,
		UNARY_EXPR,
		IDENTIFIER_EXPR,
		STRING_LITERAL_EXPR,
		BOOL_LITERAL_EXPR,
		FLOAT_LITERAL_EXPR,
		INTEGER_LITERAL_EXPR,
		ARRAY_LITERAL_EXPR,
		OBJECT_LITERAL_EXPR,
		CALL_EXPR,
		MEMBER_ACCESS_EXPR,
		INDEX_ACCESS_EXPR,
	};


	// list of all the ast node types
	struct expr; // abstract class

	struct program_expr;

	struct function_declaration_expr;

	struct variable_declaration_expr;

	struct block_expr;

	struct if_expr;
	struct while_expr;
	struct for_expr;
	// only in function scope
	struct return_expr;
	// only in loop scope
	struct break_expr;
	struct continue_expr;

	struct binary_expr;
	struct unary_expr;
	struct call_expr;
	struct member_access_expr;
	struct index_access_expr;

	struct identifier_expr;
	//literals
	struct string_literal_expr;
	struct bool_literal_expr;
	struct integer_literal_expr;
	struct float_literal_expr;
	struct array_literal_expr;
	struct object_literal_expr;
		
	// implemention

	struct expr
	{
		expr_type type = (expr_type)-1;

		//virtual ~expr() = 0;
	};

	struct program_expr : expr
	{
		vector<unique_ptr<expr>> body;
	};

	
	struct function_declaration_expr : expr
	{
		string identifier;
		vector<unique_ptr<expr>> args;
		data_type return_type;
		unique_ptr<expr> body;
	};

	struct variable_declaration_expr : expr
	{
		string identifier;
		data_type d_type;
		var_type v_type;
		unique_ptr<expr> value;
	};	

	struct block_expr : expr
	{
		vector<unique_ptr<expr>> body;
	};

	struct if_expr : expr
	{
		unique_ptr<expr> condition, then, _else;
	};

	struct while_expr : expr
	{
		unique_ptr<expr> condition;
		unique_ptr<expr> body;
	};

	struct for_expr : expr
	{
		unique_ptr<expr> init_statement;
		vector<unique_ptr<expr>> update_statements;
		unique_ptr<expr> condition;
		unique_ptr<expr> body;
	};

	struct return_expr : expr
	{
		unique_ptr<expr> return_value;
	};

	struct break_expr : expr
	{
		// no members
	};

	struct continue_expr : expr
	{
		// no members
	};

	struct binary_expr : expr
	{
		lexical::token_type _operator;
		unique_ptr<expr> left, right;
	};

	struct unary_expr : expr
	{
		lexical::token_type _operator;
		unique_ptr<expr> operand;
	};

	struct identifier_expr : expr
	{
		string identifier;
	};

	struct string_literal_expr : expr
	{
		string value;
	};

	struct bool_literal_expr : expr
	{
		bool value; 
	};

	struct float_literal_expr : expr
	{
		ldouble_t value;
	};

	struct integer_literal_expr : expr
	{
		int64_t value;
	};

	struct array_literal_expr : expr
	{
		vector<unique_ptr<expr>> values;
	};

	struct object_literal_expr : expr
	{
		vector<pair<unique_ptr<expr>, unique_ptr<expr>>> pairs;
	};

	struct call_expr : expr
	{
		unique_ptr<expr> callee;
		vector<unique_ptr<expr>> args;
	};

	struct member_access_expr : expr
	{
		unique_ptr<expr> object;
		string memeber_identifier;
	};

	struct index_access_expr : expr
	{
		unique_ptr<expr> object;
		unique_ptr<expr> index;
	};
}
