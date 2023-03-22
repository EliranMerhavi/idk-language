#pragma once

#include "typedefs.h"
#include "ast.h"
#include "lexical/tokenizer.h"


namespace syntax
{
	using namespace std;
	/// <summary>
	/// this class responsible for creating
	/// the abstract syntax tree from a token 
	/// sequence of a given program
	/// </summary>
	class parser
	{
		/// <summary>
		/// used for determining on which scope
		/// the parser is in
		/// </summary>
		using scope_type = enum
		{
			FUNCTION_SCOPE,
			LOOP_SCOPE
		};

	public:
		parser(); // default constructor

		/// <summary>
		/// used for parsing ast::program nodes
		/// </summary>
		/// <returns>ast::program node</returns>
		auto parse_program(const std::vector<std::string>& source) -> unique_ptr<ast::expr>;

	private: // parse methods
	

		/// <summary>
		/// used for parsing ast::block_expr node and statement nodes
		/// </summary>
		/// <returns>ast::block_expr or statement node</returns>
		auto parse_block_or_statement()->unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::block_expr nodes
		/// </summary>
		/// <returns>ast::block_expr node</returns>
		auto parse_block() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing statement nodes
		/// these are the statements nodes:
		///		* ast::function_declaration_expr
		///		* ast::variable_declaration_expr
		///		* ast::if_expr
		///		* ast::while_expr
		///		* ast::for_expr
		///
		/// </summary>
		/// <returns>statement node</returns>
		auto parse_statement() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::function_declaration_expr nodes
		/// </summary>
		/// <returns>ast::function_declaration_expr node</returns>
		auto parse_function()->unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::variable_declaration_expr nodes
		/// </summary>
		/// <param name="support_default_value"></param>
		/// <returns>ast::variable_declaration_expr node</returns>
		auto parse_variable_declaration(bool support_default_value)->unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::if_expr nodes
		/// </summary>
		/// <returns>ast::if_expr node</returns>
		auto parse_if_statement() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::while_expr nodes
		/// </summary>
		/// <returns>ast::while_expr node</returns>
		auto parse_while_statement() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::for_expr nodes
		/// </summary>
		/// <returns>ast::for_expr node</returns>
		auto parse_for_statement() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::return_expr nodes
		/// </summary>
		/// <returns>ast::return_expr node</returns>
		auto parse_return_statement() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::break_expr and ast::continue_expr nodes
		/// </summary>
		/// <returns>ast::break_expr or ast::continue_expr nodes</returns>
		auto parse_special_loop_statement() -> unique_ptr<ast::expr>; // can be continue or break


		/// <summary>
		/// used for parsing evaluable expression nodes
		/// </summary>
		/// <returns>evaluable expression node</returns>
		auto parse_expression() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::binary_expr nodes.
		/// this function gets the left operand 
		/// of the binary exprssion and the precendence 
		/// of the left operand
		/// </summary>
		/// <param name="expr_prec">the precedence of left</param>
		/// <param name="left">the left operand</param>
		/// <returns>ast::binary_expr or ast::unary_expr or primary expression node</returns>
		auto parse_binary_expression(int expr_prec, std::unique_ptr<ast::expr>& left) ->std::unique_ptr<ast::expr>;
		
		
		/// <summary>
		/// used for parsing ast::unary_expr nodes
		/// </summary>
		/// <returns>ast::unary_expr or primary expression node</returns>
		auto parse_unary_expr() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing primary expression nodes
		/// these are the primary expression nodes:
		///		* literal node
		///		* parenthesized expression
		/// </summary>
		/// <returns>evaluable expression or literal node</returns>
		auto parse_primary_expression() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::call_expr, ast::member_access_expr
		/// and ast::index_access_expr nodes
		/// </summary>
		/// <param name="object"></param>
		/// <returns>ast::call_expr or ast::member_access_expr or 
		/// ast::index_access_expr node</returns>
		auto parse_call_and_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>;
		
		
		/// <summary>
		/// used for parsing ast::member_access_expr nodes
		/// </summary>
		/// <param name="object"></param>
		/// <returns>ast::member_access_expr node</returns>
		auto parse_member_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::index_access_expr nodes
		/// </summary>
		/// <param name="object">the object node</param>
		/// <returns>ast::index_access_expr node</returns>
		auto parse_index_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::call_expr nodes
		/// </summary>
		/// <param name="callee">the callee node</param>
		/// <returns>ast::call_expr node</returns>
		auto parse_call_expression(unique_ptr<ast::expr>& callee) -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::identifier_expr nodes
		/// </summary>
		/// <returns>ast::identifier_expr node</returns>
		auto parse_identifier() -> unique_ptr<ast::expr>;


		// literals stuff
		/// <summary>
		/// used for parsing ast::float_literal nodes
		/// </summary>
		/// <returns>ast::float_literal node</returns>
		auto parse_float_literal() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::integer_literal nodes
		/// </summary>
		/// <returns>ast::integer_literal node</returns>
		auto parse_integer_literal() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::string_literal nodes
		/// </summary>
		/// <returns>ast::string_literal node</returns>
		auto parse_string_literal() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::bool_expr nodes
		/// </summary>
		/// <returns>ast::bool_expr node</returns>
		auto parse_bool_literal() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::object_literal nodes
		/// </summary>
		/// <returns>ast::object_literal node</returns>
		auto parse_object_literal() -> unique_ptr<ast::expr>;


		/// <summary>
		/// used for parsing ast::array_literal nodes
		/// </summary>
		/// <returns>ast::array_literal node</returns>
		auto parse_array_literal() -> unique_ptr<ast::expr>;

	private: // utlity methods
		/// <summary>
		/// returns lookahead token
		/// and assign the next token to lookahead
		/// if lookahead.type != token_type then the fucntion
		/// throws expection
		/// </summary>
		/// <param name="token_type">the token type that supposed to be parsed</param>
		/// <returns>lookahead value</returns>
		auto eat(lexical::token_type token_type) -> lexical::token;
		

		/// <summary>
		/// returns the precedence of a given token type
		/// </summary>
		/// <param name="token_type"></param>
		/// <returns>the precedence of a given token type</returns>
		static auto get_token_precedence(lexical::token_type token_type) -> int;

		/// <summary>
		/// used to notify an entrance 
		/// of a scope 
		/// </summary>
		/// <param name="scope">can be LOOP or FUNCTION</param>
		/// <returns>nothing</returns>
		inline auto enter_scope(scope_type scope) -> void
		{
			scopes[scope].push(true);
		}

		/// <summary>
		/// used to notify an exit of 
		/// a scope
		/// </summary>
		/// <param name="scope">can be LOOP or FUNCTION</param>
		/// <returns>nothing</returns>
		inline auto exit_scope(scope_type scope) -> void
		{
			scopes[scope].pop();
		}

		/// <summary>
		/// used for checking if the parser
		/// is in a current scope
		/// </summary>
		/// <param name="scope">can be LOOP or FUNCTION</param>
		/// <returns>true if is in scope otherwise false</returns>
		auto is_in_scope(scope_type scope) -> bool
		{
			return !scopes[scope].empty();
		}

	private:
		// used for tracking on which scope the parser is in
		std::stack<bool> scopes[3]; 
		// used for creating the token sequence 
		lexical::tokenizer tokenizer; 
		// used for holding the current token
		lexical::token lookahead;
		// the program source
		std::string source;
	};
}
