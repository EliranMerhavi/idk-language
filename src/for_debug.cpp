#include "pch.h"
#include "lexical/token_type.h"

namespace debug
{
	using namespace ast;

	void indent_line(int indent, int depth)
	{
		for (int _ = 0; _ < indent * depth; _++)
		{
			std::cout << ' ';
		}
	}

	auto operator_to_string(lexical::token_type type) -> std::string
	{
		std::string str;
		switch (type)
		{
		case lexical::token_type::ADD:
			str = "+";
			break;
		case lexical::token_type::SUB:
			str = "-";
			break;
		case lexical::token_type::MULT:
			str = "*";
			break;
		case lexical::token_type::DIV:
			str = "/";
			break;
		case lexical::token_type::EQUAL:
			str = "==";
			break;
		case lexical::token_type::NEQUAL:
			str = "!=";
			break;
		case lexical::token_type::ASSIGN:
			str = "=";
			break;
		case lexical::token_type::XOR:
			str = "^";
			break;
		case lexical::token_type::BITWISE_OR:
			str = "|";
			break;
		case lexical::token_type::BITWISE_AND:
			str = "&";
			break;
		case lexical::token_type::BITWISE_NEG:
			str = "~";
			break;
		case lexical::token_type::LOGICAL_OR:
			str = "||";
			break;
		case lexical::token_type::LOGICAL_AND:
			str = "&&";
			break;
		case lexical::token_type::LOGICAL_NEG:
			str = "!";
			break;
		case lexical::token_type::LESSER:
			str = "<";
			break;
		case lexical::token_type::BIGGER:
			str = ">";
			break;
		case lexical::token_type::LESSER_EQUALS:
			str = "<=";
			break;
		case lexical::token_type::BIGGER_EQUALS:
			str = ">=";
			break;
		case lexical::token_type::SHIFT_LEFT:
			str = "<<";
			break;
		case lexical::token_type::SHIFT_RIGHT:
			str = ">>";
			break;
		case lexical::token_type::MODULO:
			str = "%";
			break;
		default:
			throw std::exception("unknown operator");
			break;
		}

		return str;
	}

	void print_tree(const unique_ptr<expr>& root, int indent, int depth, bool new_line)
	{
		if (!root) return;

		switch (root->type)
		{
		case ast::expr_type::PROGRAM_EXPR:
		{
			program_expr* temp = (program_expr*)root.get();
			for (auto& statement : temp->body)
			{
				print_tree(statement, indent, depth);
				std::cout << '\n';
			}
			break;
		}
		case ast::expr_type::FUNCTION_DECLARATION_EXPR:
		{
			function_declaration_expr* func = (function_declaration_expr*)root.get();

			std::cout << "function " << func->identifier << " (";

			auto& args = func->args;

			if (args.size())
			{
				int i;
				for (i = 0; i < args.size() - 1; i++)
				{
					print_tree(args[i], indent, depth);
					std::cout << ", ";
				}
				print_tree(args[i], indent, depth);
			}
			
			std::cout << ") -> " << data_type_to_string(func->return_type);
			print_tree(func->body, indent, depth);

			break;
		}
		case ast::expr_type::VARIABLE_DECLARATION_EXPR:
		{
			variable_declaration_expr* temp = (variable_declaration_expr*)root.get();
			indent_line(indent, depth);
			std::cout
				<< (temp->v_type == var_type::LET ? "let " : "const ")
				<< temp->identifier << ": " << data_type_to_string(temp->d_type);

			if (temp->value)
			{
				std::cout << " = ";
				print_tree(temp->value, indent, depth);
			}

			break;
		}
		case ast::expr_type::BLOCK_EXPR:
		{
			block_expr* temp = (block_expr*)root.get();
			std::cout << '\n';
			indent_line(indent, depth);
			std::cout << "{\n";
			for (auto& statement : temp->body)
			{
				indent_line(indent, depth + 1);
				print_tree(statement, indent, depth + 1);
				std::cout << ";\n";
			}
			indent_line(indent, depth);
			std::cout << '}';
			break;
		}
		case ast::expr_type::IF_EXPR:
		{
			if_expr* temp = (if_expr*)root.get();

			indent_line(indent, depth);
			std::cout << "if (";
			print_tree(temp->condition, indent, depth);
			std::cout << ")\n";
			print_tree(temp->then, indent, depth);
			if (temp->_else)
			{
				indent_line(indent, depth);
				std::cout << "else";
				print_tree(temp->_else, indent, depth);
			}
			std::cout << '\n';
			break;
		}
		case ast::expr_type::WHILE_EXPR:
		{
			while_expr* temp = (while_expr*)root.get();
			indent_line(indent, depth);
			std::cout << "while (";
			print_tree(temp->condition, indent, depth);

			std::cout << ")";
			print_tree(temp->body, indent, depth);
			std::cout << '\n';
			break;
		}
		case ast::expr_type::FOR_EXPR:
		{
			indent_line(indent, depth);
			for_expr* temp = (for_expr*)root.get();

			std::cout << "for (";
			print_tree(temp->init_statement, indent, depth);
			std::cout << "; ";
			print_tree(temp->condition, indent, depth);
			std::cout << "; ";

			auto& update_statements = temp->update_statements;

			if (update_statements.size())
			{
				int i;
				for (i = 0; i < update_statements.size() - 1; i++)
				{
					print_tree(update_statements[i], indent, depth);
					std::cout << ", ";
				}
				print_tree(update_statements[i], indent, depth);
			}

			std::cout << ")";
			print_tree(temp->body, indent, depth);
			std::cout << '\n';
			break;
		}
		case ast::expr_type::RETURN_EXPR:
		{
			return_expr* temp = (return_expr*)root.get();
			indent_line(indent, depth);
			std::cout << "return ";

			print_tree(temp->return_value, indent, depth);
			break;
		}
		case ast::expr_type::BREAK_EXPR:
			std::cout << "break";
			break;
		case ast::expr_type::CONTINUE_EXPR:
			std::cout << "continue";
			break;
		case ast::expr_type::BINARY_EXPR:
		{
			binary_expr* temp = (binary_expr*)root.get();
			std::cout << "( ";
			print_tree(temp->left, indent, depth + 1);
			std::cout << ' ' + operator_to_string(temp->_operator) + ' ';
			print_tree(temp->right, indent, depth + 1);
			std::cout << " )";
			break;
		}
		case ast::expr_type::UNARY_EXPR:
		{
			unary_expr* temp = (unary_expr*)root.get();
			cout << operator_to_string(temp->_operator);
			print_tree(temp->operand, indent, depth + 1);
			break;
		}
		case ast::expr_type::IDENTIFIER_EXPR:
			std::cout << ((identifier_expr*)root.get())->identifier;
			break;
		case ast::expr_type::STRING_LITERAL_EXPR:
			std::cout << "\"" + ((string_literal_expr*)root.get())->value + "\"";
			break;
		case ast::expr_type::BOOL_LITERAL_EXPR:
			std::cout << ((bool_literal_expr*)root.get())->value ? "true" : "false";
			break;
		case ast::expr_type::FLOAT_LITERAL_EXPR:
			std::cout << ((float_literal_expr*)root.get())->value;
			break;
		case ast::expr_type::INTEGER_LITERAL_EXPR:
			std::cout << ((integer_literal_expr*)root.get())->value;
			break;
		case ast::expr_type::ARRAY_LITERAL_EXPR:
		{
			array_literal_expr* temp = (array_literal_expr*)root.get();

			auto& values = temp->values;

			std::cout << "[";
			if (values.size())
			{
				int i;
				for (i = 0; i < values.size() - 1; i++)
				{
					print_tree(values[i], indent, depth);
					std::cout << ", ";
				}
				print_tree(values[i], indent, depth);
			}

			std::cout << "]";

			break;
		}
		case ast::expr_type::OBJECT_LITERAL_EXPR:
		{
			object_literal_expr* temp = (object_literal_expr*)root.get();

			auto& pairs = temp->pairs;

			std::cout << "{";
			if (pairs.size())
			{
				int i;
				
				for (i = 0; i < pairs.size() - 1; i++)
				{
					auto& [key, value] = pairs[i];

					print_tree(key, indent, depth);
					std::cout << " : ";
					print_tree(value, indent, depth);
					std::cout << ", ";
				}

				auto& [key, value] = pairs[i];

				print_tree(key, indent, depth);
				std::cout << " : ";
				print_tree(value, indent, depth);
			}
			std::cout << "}";

			break;
		}
		case ast::expr_type::CALL_EXPR:
		{
			call_expr* temp = (call_expr*)root.get();
			indent_line(indent, depth);
			auto& args = temp->args;

			print_tree(temp->callee, indent, depth);

			std::cout << "(";
			if (args.size())
			{
				int i;
				for (i = 0; i < args.size() - 1; i++)
				{
					print_tree(args[i], indent, depth);
					std::cout << ", ";
				}
				print_tree(args[i], indent, depth);
			}

			std::cout << ")";

			break;
		}
		case ast::expr_type::MEMBER_ACCESS_EXPR:
		{
			member_access_expr* temp = (member_access_expr*)root.get();
			print_tree(temp->object, indent, depth);
			std::cout << "." << temp->memeber_identifier;
			break;
		}
		case ast::expr_type::INDEX_ACCESS_EXPR:
		{
			index_access_expr* temp = (index_access_expr*)root.get();
			print_tree(temp->object, indent, depth);
			cout << '[';
			print_tree(temp->index, indent, depth);
			cout << ']';
			break;
		}
		}

		if (new_line)
			std::cout << '\n';
	}
}
