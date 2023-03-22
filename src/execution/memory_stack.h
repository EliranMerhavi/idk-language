#pragma once

#include "variables/abstract_var.h"

namespace execution
{
	/// <summary>
	/// this class is responsible for
	/// storage of named varaibles
	/// </summary>
	class memory_stack
	{
		using stack_memory_item_t = struct {
			bool is_function_scope;
			std::unordered_map<std::string, std::shared_ptr<abstract_var>> variables;
		};
	public:
		memory_stack(); // defualt constructor

		/// <summary>
		/// attempts to find a named variable that his name is equal identifier
		/// in the current scope and returns it
		/// if the variable was not found it throws a runtime error
		/// </summary>
		/// <param name="identifier">the name of the variable</param>
		/// <returns>the variable</returns>
		auto get_var(const std::string& identifier) -> std::shared_ptr<abstract_var>;
		
		/// <summary>
		/// creates a variable on the stack or in the global variables
		/// (depends on the scope) if variable with the name identifier is already exists 
		/// then the funcion throws a runtime error
		/// </summary>
		/// <param name="identifier"></param>
		/// <param name="value"></param>
		/// <returns></returns>
		auto create_var(const std::string& identifier, const std::shared_ptr<abstract_var>& value) -> void;
		
		inline auto push_scope(bool is_block_scope) -> void
		{
			_stack.emplace_back(is_block_scope, std::unordered_map<std::string, std::shared_ptr<abstract_var>>());
		}

		inline auto pop_scope() -> void
		{
			_stack.pop_back();
		}
			
		inline auto is_function_scope() -> bool
		{
			return !_stack.empty() && _stack.back().is_function_scope;
		}

	private:
		/// <summary>
		/// global variables that are stored throughout all the runtime of the program
		/// </summary>
		static std::unordered_map<std::string, std::shared_ptr<abstract_var>> globals;
		static bool is_globals_initialized;
	private:
		/// <summary>
		/// the actual stack of the memory stack
		/// </summary>
		std::vector<stack_memory_item_t> _stack;
	};

}
