#pragma once

#include "syntax/ast.h"
#include "syntax/parser.h"
#include "memory_stack.h"
#include "variables/void_var.h"


#define MAX_CALL_STACK_SIZE 50


namespace execution
{
	class executer
	{
	public:
		executer();
		~executer();

		auto execute_file(const std::string& filepath) -> void;
		auto execute(const std::vector<std::string>& source) -> void;
		auto execute_func(const std::unordered_map<std::string, std::shared_ptr<abstract_var>>& args, const ast::function_declaration_expr* body) -> void;

		inline auto get_return_value() -> std::shared_ptr<abstract_var>
		{
			const auto temp = return_value;
			return_value = void_var::instance();
			return temp;
		}
	private:

		auto execute_program(const ast::program_expr* program_node) -> void;
		auto execute_function_declaration(const ast::function_declaration_expr* node) -> void;

		auto execute_statement_or_block(const ast::expr* node) -> void;
		auto execute_statement(const ast::expr* node) -> void;
		auto execute_block(const ast::block_expr* block_node) -> void;

		auto eval(const ast::expr* node) -> std::shared_ptr<abstract_var>;
		auto eval_call(const ast::call_expr* node) -> std::shared_ptr<abstract_var>;

		
	private:

		execution::memory_stack memory;
		syntax::parser parser;
		bool break_flag, continue_flag, return_value_flag;	
		std::stack<std::string> call_stack;
		std::shared_ptr<abstract_var> return_value;
	};
}