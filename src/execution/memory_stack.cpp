#include "pch.h"
#include "memory_stack.h"

#include "variables/abstract_var.h"
#include "variables/func_var.h"
#include "variables/integer_var.h"
#include "variables/string_var.h"
#include "variables/bool_var.h"
#include "variables/float_var.h"
#include "variables/void_var.h"
#include "variables/var_utils.h"

namespace execution
{
	std::unordered_map<std::string, std::shared_ptr<abstract_var>> memory_stack::globals = {};
	bool memory_stack::is_globals_initialized = false;

	memory_stack::memory_stack()
	{
		if (!is_globals_initialized) 
		{
			// init globals

			globals["RAND_MAX"] = std::make_shared<integer_var>(RAND_MAX);
			
			globals["print"] = std::make_shared<func_var>([](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {

				for (const auto& arg : args)
					std::cout << *arg << ' ';

				std::cout << '\n';

				return void_var::instance();
			}, "print", INFINITE_ARGS_SIZE);

			globals["input"] = std::make_shared<func_var>([=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
				std::string line;

				const auto& var = args.front();

				interpeter_assert(var->is_named_var(), "argument in function 'input' must be a named var", RUNTIME_ERROR);

				std::getline(std::cin, line);
				try
				{
					switch (var->get_data_type())
					{
					case data_type::INTEGER:
						*var = std::make_shared<integer_var>(std::stoll(line));
						break;

					case data_type::FLOAT:
						*var = std::make_shared<float_var>(std::stold(line));
						break;

					case data_type::BOOL:
						interpeter_assert(line == "true" || line == "false", "bool input can only be 'true' or 'false'", RUNTIME_ERROR);
						*var = std::make_shared<bool_var>(line == "true");
						break;

					case data_type::STRING:
						*var = std::make_shared<string_var>(line);
						break;

					default:
						return std::make_shared<bool_var>(false);
						break;
					}
				}
				catch (const interpeter_error&)
				{
					return std::make_shared<bool_var>(false);
				}

				return std::make_shared<bool_var>(true);
			}, "input", 1);

			globals["rand"] = std::make_shared<func_var>([](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
				return std::make_shared<integer_var>(rand());
			}, "rand", 0);

			globals["tanh"] = std::make_shared<func_var>([](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
				return std::make_shared<float_var>(tanh((ldouble_t)*args[0]));
			}, "tanh", 1);

			is_globals_initialized = true;
		}
	}

	auto memory_stack::get_var(const std::string& identifier) -> std::shared_ptr<abstract_var>
	{
		std::shared_ptr<abstract_var> var = nullptr;

		if (globals.find(identifier) != globals.end())
		{
			var = globals[identifier];
		}
		else if (!_stack.empty())
		{
			int64_t i;
			for (i = _stack.size() - 1; i >= 0 && !var; i--)
			{
				if (_stack[i].variables.find(identifier) != _stack[i].variables.end())
				{
					var = _stack[i].variables[identifier];
				}

				if (_stack[i].is_function_scope)
					break;
			}
		}

		interpeter_assert(var, ("'" + identifier + "' doenst exists").c_str(), RUNTIME_ERROR);

		return var;
	}

	auto memory_stack::create_var(const std::string& identifier, const std::shared_ptr<abstract_var>& value) -> void
	{
		if (_stack.empty())
		{
			interpeter_assert(globals.find(identifier) == globals.end(), ("'" + identifier + "' already exists in this scope").c_str(), RUNTIME_ERROR);
			globals[identifier] = value;
			value->set_identifier(identifier);
		}
		else
		{
			interpeter_assert(_stack.back().variables.find(identifier) == _stack.back().variables.end(), ("'" + identifier + "' already exists in this scope").c_str(), RUNTIME_ERROR);
			_stack.back().variables[identifier] = value;
			value->set_identifier(identifier);
		}
	}
}