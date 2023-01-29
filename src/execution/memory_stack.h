#pragma once

#include "variables/abstract_var.h"

namespace execution
{
	class memory_stack
	{
		using stack_memory_t = struct {
			bool is_function_scope;
			std::unordered_map<std::string, std::shared_ptr<abstract_var>> variables;
		};
	public:
		memory_stack();

		auto get_var(const std::string& identifier) -> std::shared_ptr<abstract_var>;
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
			return _stack.back().is_function_scope;
		}

	private:
		std::vector<stack_memory_t> _stack;
		static std::unordered_map<std::string, std::shared_ptr<abstract_var>> globals;
		static bool is_globals_initialized;
	};

}
