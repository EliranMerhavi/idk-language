#pragma once

#include "syntax/ast.h"
#include "syntax/parser.h"
#include "memory_stack.h"
#include "variables/void_var.h"


#define MAX_CALL_STACK_SIZE 50

namespace execution
{
	/// <summary>
	/// this class is responsible for executing 
	/// code that written in the idk language syntax
	/// the syntax of the language is described in info/idk.grammer
	/// </summary>
	class executer
	{
	public:
		executer(); // default constuctor
		~executer(); // defualt destructor

		/// <summary>
		/// executes a file content
		/// </summary>
		/// <param name="filepath">the filepath to the file</param>
		/// <returns>nothing</returns>
		auto execute_file(const std::string& filepath) -> void;


		/// <summary>
		/// executes the source file
		/// </summary>
		/// <param name="source"></param>
		/// <returns>nothing</returns>
		auto execute(const std::vector<std::string>& source) -> void;


		/// <summary>
		/// executes a function with named arguments
		/// </summary>
		/// <param name="args">the arguemnts</param>
		/// <param name="func_node">the ast represntation of the function</param>
		/// <returns>nothing</returns>
		auto execute_func(const std::unordered_map<std::string, std::shared_ptr<abstract_var>>& args, const ast::function_declaration_expr* func_node) -> void;

		/// <summary>
		/// getter for return_value. in the proccess this function resets return_value 
		/// </summary>
		/// <returns>return_value</returns>
		inline auto get_return_value() -> std::shared_ptr<abstract_var>
		{
			const auto temp = return_value;
			return_value = void_var::instance();
			return temp;
		}
	private:
		/// <summary>
		/// executes a program
		/// </summary>
		/// <param name="program_node">the ast representation of the program</param>
		/// <returns>nothing</returns>
		auto execute_program(const ast::program_expr* program_node) -> void;
		
		
		/// <summary>
		/// this function gets an ast of a block or an ast of a statement
		/// and executes it
		/// </summary>
		/// <param name="node">the node to execute</param>
		/// <returns>nothing</returns>
		auto execute_statement_or_block(const ast::expr* node) -> void;

		/// <summary>
		/// executes an ast represntation of a statement
		/// </summary>
		/// <param name="node">the ast represntation of the statement</param>
		/// <returns></returns>
		auto execute_statement(const ast::expr* node) -> void;


		/// <summary>
		/// executes a block 
		/// </summary>
		/// <param name="block_node">the ast represntation of the block</param>
		/// <returns></returns>
		auto execute_block(const ast::block_expr* block_node) -> void;


		/// <summary>
		/// this function gets a evaluable expression tree
		/// and returns the evaluated result
		/// </summary>
		/// <param name="node">the ast to evaluate</param>
		/// <returns></returns>
		auto eval(const ast::expr* node) -> std::shared_ptr<abstract_var>;
	private:
		/// <summary>
		/// internal flags of the executer
		/// </summary>
		bool break_flag,
			continue_flag,
			return_value_flag;
			
		execution::memory_stack memory; // for storing the values
		syntax::parser parser; // for parsing the ast of the program

		//TODO move these to the memory_stack
		std::stack<std::string> call_stack;
		std::shared_ptr<abstract_var> return_value;
	};
}