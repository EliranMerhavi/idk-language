#include "pch.h"
#include "executer.h"

#include "variables/integer_var.h"
#include "variables/string_var.h"
#include "variables/bool_var.h"
#include "variables/float_var.h"
#include "variables/abstract_var.h"
#include "variables/func_var.h"
#include "variables/array_var.h"
#include "variables/object_var.h"
#include "variables/var_utils.h"

namespace execution
{
	executer::executer()
		: parser(), 
		  break_flag(false), 
		  continue_flag(false), 
		  return_value_flag(false),
		  memory(),
		  return_value()
	{
		
	}

	executer::~executer()
	{
	}

	auto executer::execute_file(const std::string& filepath) -> void
	{
		try
		{
			execute(utils::load_source(filepath));
		}
		catch (const interpeter_error& e)
		{
			std::cerr << e.msg << '\n';
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}

	auto executer::execute(const std::vector<std::string>& source) -> void
	{
		const std::unique_ptr<ast::expr>& program_node = parser.parse_program(source);
		
		try
		{
			execute_program((ast::program_expr*)program_node.get());
		}
		catch (const interpeter_error& e)
		{
			// print stacktrace
			while (!call_stack.empty())
			{
				std::cerr << "in function: " << call_stack.top() << '\n';
				call_stack.pop();
			}
			throw e;
		}
	}

	auto executer::execute_program(const ast::program_expr* program_node) -> void
	{
		for (const std::unique_ptr<ast::expr>& statement : program_node->body)
			execute_statement(statement.get());
	}

	auto executer::execute_func(const std::unordered_map<std::string, std::shared_ptr<abstract_var>>& args, const ast::function_declaration_expr* func_node) -> void
	{
		interpeter_assert(call_stack.size() < MAX_CALL_STACK_SIZE, "stackoverflow error", SYNTAX_ERROR);
		memory.push_scope(true);
		call_stack.push(func_node->identifier);
		for (const auto &[identifier, value] : args)
			memory.create_var(identifier, value);
		
		execute_block((ast::block_expr*)func_node->body.get());
		
		interpeter_assert(return_value->get_data_type() == func_node->return_type, ("unmatching return value type expected '" +
			data_type_to_string(func_node->return_type) + "', got '" + data_type_to_string(return_value->get_data_type()) + "'").c_str(),
			RUNTIME_ERROR);

		call_stack.pop();
		memory.pop_scope();
	}

	auto executer::execute_statement_or_block(const ast::expr* node) -> void
	{
		if (node)
		{
			switch (node->type)
			{
			case ast::expr_type::BLOCK_EXPR:
				execute_block((ast::block_expr*)node);
				break;
			default:
				execute_statement(node);
				break;
			}
		}
	}
	
	auto executer::execute_block(const ast::block_expr* block_node) -> void
	{
		memory.push_scope(false);
		
		for (auto it = block_node->body.begin(); 
			!continue_flag && !break_flag && !return_value_flag &&
			it != block_node->body.end(); 
			it = next(it))
		{
			execute_statement(it->get());
		}

		memory.pop_scope();
		
		if (memory.is_function_scope() && return_value_flag)
			return_value_flag = false;
		if (continue_flag)
			continue_flag = false;

	}

	auto executer::execute_statement(const ast::expr* node) -> void
	{
		switch (node->type)
		{
		case ast::expr_type::FUNCTION_DECLARATION_EXPR:
		{
			const ast::function_declaration_expr* dnode = (ast::function_declaration_expr*)node;
			memory.create_var(dnode->identifier, std::make_shared<func_var>(dnode, var_type::CONST));
			break;
		}
		case ast::expr_type::VARIABLE_DECLARATION_EXPR:
		{
			const ast::variable_declaration_expr* dnode = (ast::variable_declaration_expr*)node;
			if (dnode->value.get())
				memory.create_var(dnode->identifier, init_var(dnode->d_type, dnode->v_type, eval(dnode->value.get())));
			else
				memory.create_var(dnode->identifier, init_var(dnode->d_type, dnode->v_type));
			break;
		}
		case ast::expr_type::IF_EXPR:
		{
			const ast::if_expr* if_node = (ast::if_expr*)node;
			if (*eval(if_node->condition.get()))
				execute_statement_or_block(if_node->then.get());
			else
				execute_statement_or_block(if_node->_else.get());
			break;
		}
		case ast::expr_type::FOR_EXPR:
		{
			const ast::for_expr* for_node = (ast::for_expr*)node;
			memory.push_scope(false);
			for (execute_statement(for_node->init_statement.get()); // init statement
				// condition
				!break_flag && !return_value_flag && *eval(for_node->condition.get());	
				// update statement
				std::for_each(for_node->update_statements.begin(), for_node->update_statements.end(), 
					[=](const auto& statement){execute_statement(statement.get());})
				)
			{
				execute_statement_or_block(for_node->body.get());	
			}
			if (break_flag)
				break_flag = false;
			memory.pop_scope();
			break;
		}
		case ast::expr_type::WHILE_EXPR:
		{
			const ast::while_expr* while_node = (ast::while_expr*)node;
			
			while (*eval(while_node->condition.get()) && !break_flag && !return_value_flag)
			{
				execute_statement_or_block(while_node->body.get());
			}
			if (break_flag)
				break_flag = false;
			break;
		}
		case ast::expr_type::RETURN_EXPR:
		{
			const ast::expr* return_value_node = ((ast::return_expr*)node)->return_value.get();			
			return_value = return_value_node ? eval(return_value_node) : void_var::instance();			
			return_value_flag = true;
			break;
		}
		case ast::expr_type::BREAK_EXPR:
			break_flag = true;
			break;
		case ast::expr_type::CONTINUE_EXPR:
			continue_flag = true;
			break;
		case (ast::expr_type)-1:
			// should be ignored
			break;
		default:
			eval(node);
			break;
		}
	}

	auto executer::eval(const ast::expr* node) -> std::shared_ptr<abstract_var>
	{
		std::shared_ptr<abstract_var> res;
		switch (node->type)
		{
		case ast::expr_type::STRING_LITERAL_EXPR:
			res = std::make_shared<string_var>(((ast::string_literal_expr*)node)->value);
			break;
		case ast::expr_type::BOOL_LITERAL_EXPR:
			res = std::make_shared<bool_var>(((ast::bool_literal_expr*)node)->value);
			break;
		case ast::expr_type::FLOAT_LITERAL_EXPR:
			res = std::make_shared<float_var>(((ast::float_literal_expr*)node)->value);
			break;
		case ast::expr_type::INTEGER_LITERAL_EXPR:
			res = std::make_shared<integer_var>(((ast::integer_literal_expr*)node)->value);
			break;
		case ast::expr_type::ARRAY_LITERAL_EXPR:
		{
			//TODO: check
			array_var::array_t value;
			for (const std::unique_ptr<ast::expr>& element : ((ast::array_literal_expr*)node)->values)
			{
				value.emplace_back(eval(element.get()));
			}

			res = std::make_shared<array_var>(value);
			break;
		}
		case ast::expr_type::OBJECT_LITERAL_EXPR:
		{
			object_var::map_t value;

			for (const auto&[key, element] : ((ast::object_literal_expr*)node)->pairs)
			{
				value[eval(key.get())] = eval(element.get());
			}
			
			res = std::make_shared<object_var>(value);
			break;
		}
		case ast::expr_type::BINARY_EXPR:
		{
			const ast::binary_expr* bin_node = (ast::binary_expr*)node;
			
			lexical::token_type op = bin_node->_operator;
			const std::shared_ptr<abstract_var>& a = eval(bin_node->left.get());
			const std::shared_ptr<abstract_var>& b = eval(bin_node->right.get());

			switch (op)
			{
			case lexical::token_type::ADD:
				res = a->operator+(b);
				break;
			case lexical::token_type::SUB:
				res = a->operator-(b);
				break;
			case lexical::token_type::MULT:
				res = a->operator*(b);
				break;
			case lexical::token_type::DIV:
				res = a->operator/(b);
				break;
			case lexical::token_type::EQUAL:
				res = a->operator==(b);
				break;
			case lexical::token_type::NEQUAL:
				res = a->operator!=(b);
				break;
			case lexical::token_type::ASSIGN:
				*a = b;
				res = a;
				break;
			case lexical::token_type::XOR:
				res = a->operator^(b);
				break;
			case lexical::token_type::BITWISE_OR:
				res = a->operator|(b);
				break;
			case lexical::token_type::BITWISE_AND:
				res = a->operator&(b);
				break;
			case lexical::token_type::LOGICAL_OR:
				res = a->operator||(b);
				break;
			case lexical::token_type::LOGICAL_AND:
				res = a->operator&&(b);
				break;
			case lexical::token_type::LESSER:
				res = a->operator<(b);
				break;
			case lexical::token_type::BIGGER:
				res = a->operator>(b);
				break;
			case lexical::token_type::LESSER_EQUALS:
				res = a->operator<=(b);
				break;
			case lexical::token_type::BIGGER_EQUALS:
				res = a->operator>=(b);
				break;
			case lexical::token_type::SHIFT_LEFT:
				res = a->operator<<(b);
				break;
			case lexical::token_type::SHIFT_RIGHT:
				res = a->operator>>(b);
				break;
			case lexical::token_type::MODULO:
				res = a->modulo(b);
				break;
			default:
				break;
			}
			break;
		}
		case ast::expr_type::UNARY_EXPR:
		{
			const ast::unary_expr* unary_node = (ast::unary_expr*)node;
			lexical::token_type op = unary_node->_operator;
			const std::shared_ptr<abstract_var>& a = eval(unary_node->operand.get());

			switch (op)
			{
			case lexical::token_type::BITWISE_NEG:
				res = a->operator~();
				break;
			case lexical::token_type::LOGICAL_NEG:
				res = a->operator!();
				break;
			case lexical::token_type::ADD:
				res = a->operator+();
				break;
			case lexical::token_type::SUB:
				res = a->operator-();
				break;
			default:
				break;
			}
			break;
		}
		case ast::expr_type::IDENTIFIER_EXPR:
			res = memory.get_var(((ast::identifier_expr*)node)->identifier);
			break;

		case ast::expr_type::CALL_EXPR:
		{
			std::vector<std::shared_ptr<abstract_var>> evaled_args;
			
			const ast::call_expr* call_node = (ast::call_expr*)node;
			const std::shared_ptr<abstract_var> callee = eval(call_node->callee.get());

			for (const std::unique_ptr<ast::expr>& arg_node : call_node->args)
				evaled_args.emplace_back(eval(arg_node.get()));
			
			res = (*callee)(*this, evaled_args);
			break;
		}
		case ast::expr_type::MEMBER_ACCESS_EXPR:
		{
			const ast::member_access_expr* access_node = (ast::member_access_expr*)node;
			const std::shared_ptr<abstract_var>& object = eval(access_node->object.get());
			
			res = (*object).get_member(access_node->memeber_identifier);
			break;
		}
		case ast::expr_type::INDEX_ACCESS_EXPR:
		{
			const ast::index_access_expr* access_node = (ast::index_access_expr*)node;

			const std::shared_ptr<abstract_var>& object = eval(access_node->object.get());
			const std::shared_ptr<abstract_var>& index = eval(access_node->index.get());

			res = (*object)[index];
			break;
		}		
		default:
			break;
		}
		
		return res;
	}
}
