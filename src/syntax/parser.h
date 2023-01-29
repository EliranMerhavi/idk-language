#pragma once

#include "typedefs.h"
#include "ast.h"
#include "lexical/tokenizer.h"

enum scope_type
{
	FUNCTION_SCOPE,
	LOOP_SCOPE,
	SWITCH_SCOPE
};

namespace syntax
{
	class parser
	{
	public:
		parser();
		auto parse_program(const std::vector<std::string>& source) -> unique_ptr<ast::expr>;

	private: // parse methods

		auto parse_function() -> unique_ptr<ast::expr>;
		auto parse_variable_declaration(bool support_default_value) -> unique_ptr<ast::expr>;

		auto parse_block_or_statement() -> unique_ptr<ast::expr>;
		auto parse_block() -> unique_ptr<ast::expr>;
		auto parse_statement() -> unique_ptr<ast::expr>;

		auto parse_if_statement() -> unique_ptr<ast::expr>;
		auto parse_while_statement() -> unique_ptr<ast::expr>;
		auto parse_for_statement() -> unique_ptr<ast::expr>;

		auto parse_return_statement() -> unique_ptr<ast::expr>;
		auto parse_special_loop_statement() -> unique_ptr<ast::expr>; // can be continue or break

		auto parse_expression() -> unique_ptr<ast::expr>;
		
		auto parse_binary_expression(int expr_prec, std::unique_ptr<ast::expr>& left) ->std::unique_ptr<ast::expr>;
		auto parse_primary_expression() -> unique_ptr<ast::expr>;

		auto parse_unary_expr() -> unique_ptr<ast::expr>;
		auto parse_call_and_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>;
		auto parse_member_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>;
		auto parse_index_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>;
		auto parse_call_expression(unique_ptr<ast::expr>& callee) -> unique_ptr<ast::expr>;

		auto parse_identifier() -> unique_ptr<ast::expr>;

		// literals stuff
		auto parse_float_literal() -> unique_ptr<ast::expr>;
		auto parse_integer_literal() -> unique_ptr<ast::expr>;
		auto parse_string_literal() -> unique_ptr<ast::expr>;
		auto parse_bool_literal() -> unique_ptr<ast::expr>;
		auto parse_object_literal() -> unique_ptr<ast::expr>;
		auto parse_array_literal() -> unique_ptr<ast::expr>;

	private: // utlity methods
		auto eat(lexical::token_type token_type) -> lexical::token;
		static auto get_token_precedence(lexical::token_type token_type) -> int;

		auto enter_scope(scope_type scope) -> void;
		auto exit_scope(scope_type scope) -> void;
		auto is_in_scope(scope_type scope) -> bool;

	private:
		std::stack<bool> scopes[3];
		lexical::tokenizer tokenizer;
		lexical::token lookahead;
		std::string source;
	};
}
