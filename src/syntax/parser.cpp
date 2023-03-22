#include "pch.h"
#include "parser.h"
#include "variables/data_type.h"
#include "variables/var_type.h"

namespace syntax
{
	using namespace std;
	parser::parser() :
		tokenizer(),
		lookahead{ "", lexical::token_type::_EOF },
		source()
	{
 	}

	auto parser::parse_program(const vector<string>& source) -> unique_ptr<ast::expr>
	{
		this->tokenizer = lexical::tokenizer(source);
		this->tokenizer >> this->lookahead;

		unique_ptr<ast::program_expr> program_node = make_unique<ast::program_expr>();
		try
		{
			while (lookahead.type != lexical::token_type::_EOF)
			{
				program_node->body.emplace_back(move(parse_statement()));
			}

			program_node->type = ast::expr_type::PROGRAM_EXPR;
		}
		catch (const std::exception& e)
		{
			cout << "[SYNTAX ERROR]\n[error] " << e.what() << '\n';
		}

		return program_node;
	}

	auto parser::parse_block_or_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> node;

		if (lookahead.type == (lexical::token_type)'{')
			node = move(parse_block());
		else
			node = move(parse_statement());

		return node;
	}

	auto parser::parse_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> node;
		
		switch (lookahead.type)
		{
		case lexical::token_type::FUNCTION_KEYWORD:
			interpeter_assert(!is_in_scope(FUNCTION_SCOPE), "cant put function declaration inside a function declaration", 
				SYNTAX_ERROR);
			node = move(parse_function());
			break;

		case lexical::token_type::IF_KEYWORD:
			node = move(parse_if_statement());
			break;

		case lexical::token_type::FOR_KEYWORD:
			node = move(parse_for_statement());
			break;

		case lexical::token_type::WHILE_KEYWORD:
			node = move(parse_while_statement());
			break;

		case lexical::token_type::LET_KEYWORD:
		case lexical::token_type::CONST_KEYWORD:
			node = move(parse_variable_declaration(true));
			eat((lexical::token_type)';');
			break;

		case lexical::token_type::BREAK_KEYWORD:
		case lexical::token_type::CONTINUE_KEYWORD:
			interpeter_assert(is_in_scope(LOOP_SCOPE), "cant use 'break' or 'continue' outside a loop", SYNTAX_ERROR);
			node = move(parse_special_loop_statement());
			eat((lexical::token_type)';');
			break;

		case lexical::token_type::RETURN_KEYWORD:
			interpeter_assert(is_in_scope(FUNCTION_SCOPE), "cant use 'return' outside a function", SYNTAX_ERROR);
			node = move(parse_return_statement());
			eat((lexical::token_type)';');
			break;
		default:
			node = move(parse_expression());
			eat((lexical::token_type)';');
			break;
		}

		return node;
	}

	auto parser::parse_function() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::function_declaration_expr> function_node = make_unique<ast::function_declaration_expr>();

		eat(lexical::token_type::FUNCTION_KEYWORD);

		function_node->identifier = eat(lexical::token_type::IDENTIFIER).lexem;
		interpeter_assert(data_type_from_string(function_node->identifier) == (data_type)-1, ("'" + function_node->identifier + "' cant be an identifier").c_str(), SYNTAX_ERROR);
		
		eat((lexical::token_type)'(');

		//parse arguments
		if (lookahead.type != (lexical::token_type)')')
		{
			set<string> arg_names;
			auto arg_node = parse_variable_declaration(false);
			arg_names.insert(static_cast<ast::variable_declaration_expr*>(arg_node.get())->identifier);
			function_node->args.push_back(move(arg_node));
			
			while (lookahead.type != (lexical::token_type)')')
			{

				eat((lexical::token_type)',');
				std::unique_ptr<ast::expr> arg_node = parse_variable_declaration(false);
				arg_names.insert(static_cast<ast::variable_declaration_expr*>(arg_node.get())->identifier);
				function_node->args.push_back(move(arg_node));
			}

			interpeter_assert(arg_names.size() == function_node->args.size(), "duplicate argument names", SYNTAX_ERROR);
		}

		eat((lexical::token_type)')');

		eat(lexical::token_type::ARROW_OPERATOR);

		function_node->return_type = data_type_from_string(eat(lexical::token_type::IDENTIFIER).lexem);

		enter_scope(FUNCTION_SCOPE);
		function_node->body = move(parse_block());
		exit_scope(FUNCTION_SCOPE);

		function_node->type = ast::expr_type::FUNCTION_DECLARATION_EXPR;

		return function_node;
	}
	
	auto parser::parse_variable_declaration(bool support_default_value) -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::variable_declaration_expr> node = make_unique<ast::variable_declaration_expr>();

		switch (lookahead.type)
		{
		case lexical::token_type::CONST_KEYWORD:
			node->v_type = var_type::CONST;
			eat(lexical::token_type::CONST_KEYWORD);
			break;
		case lexical::token_type::LET_KEYWORD:
			node->v_type = var_type::LET;
			eat(lexical::token_type::LET_KEYWORD);
			break;
		default:
			throw exception("unknown variable type");
			break;
		}

		node->identifier = eat(lexical::token_type::IDENTIFIER).lexem;
		interpeter_assert(data_type_from_string(node->identifier) == (data_type)-1, ("'" + node->identifier + "' cant be an identifier").c_str(), SYNTAX_ERROR);
		eat((lexical::token_type)':');

		if ((node->d_type = data_type_from_string(eat(lexical::token_type::IDENTIFIER).lexem)) == data_type::VOID)
		{
			throw exception("variable type cant be 'void'");
		}

		if (support_default_value && lookahead.type == lexical::token_type::ASSIGN)
		{
			eat(lexical::token_type::ASSIGN);
			node->value = move(parse_expression());
		}

		node->type = ast::expr_type::VARIABLE_DECLARATION_EXPR;

		return node;
	}

	auto parser::parse_block() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::block_expr> block_node = make_unique<ast::block_expr>();

		eat((lexical::token_type)'{');

		while (lookahead.type != (lexical::token_type)'}')
		{
			block_node->body.emplace_back(move(parse_statement()));
		}

		eat((lexical::token_type)'}');

		block_node->type = ast::expr_type::BLOCK_EXPR;

		return block_node;
	}

	

	auto parser::parse_if_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::if_expr> if_node = make_unique<ast::if_expr>();

		eat(lexical::token_type::IF_KEYWORD);
		eat((lexical::token_type)'(');
		if_node->condition = move(parse_expression());
		eat((lexical::token_type)')');
		
		if_node->then = parse_block_or_statement();

		if (lookahead.type == lexical::token_type::ELSE_KEYWORD)
		{
			eat(lexical::token_type::ELSE_KEYWORD);
			if_node->_else = parse_block_or_statement();
		}

		if_node->type = ast::expr_type::IF_EXPR;

		return if_node;
	}

	auto parser::parse_while_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::while_expr> while_node = make_unique<ast::while_expr>();

		eat(lexical::token_type::WHILE_KEYWORD);
		eat((lexical::token_type)'(');

		while_node->condition = parse_expression();

		eat((lexical::token_type)')');

		enter_scope(LOOP_SCOPE);
		while_node->body = parse_block_or_statement();
		exit_scope(LOOP_SCOPE);

		while_node->type = ast::expr_type::WHILE_EXPR;

		return while_node;
	}

	auto parser::parse_for_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::for_expr> for_node = make_unique<ast::for_expr>();
		
		eat(lexical::token_type::FOR_KEYWORD);
		eat((lexical::token_type)'(');

		// parse init statement
		if (lookahead.type != (lexical::token_type)';') 
		{
			for_node->init_statement = parse_variable_declaration(true);
		}

		eat((lexical::token_type)';');
		// parse condition 
		if (lookahead.type != (lexical::token_type)';') 
		{
			for_node->condition = parse_expression();
		}

		eat((lexical::token_type)';');

		// parse update statements
		if (lookahead.type != (lexical::token_type)')')
		{
			for_node->update_statements.emplace_back(move(parse_expression()));
		}

		while (lookahead.type != (lexical::token_type)')') 
		{
			eat((lexical::token_type)',');
			for_node->update_statements.emplace_back(move(parse_expression()));
		}

		eat((lexical::token_type)')');
		
		enter_scope(LOOP_SCOPE);
		for_node->body = parse_block_or_statement();
		exit_scope(LOOP_SCOPE);

		for_node->type = ast::expr_type::FOR_EXPR;

		return for_node;
	}


	auto parser::parse_return_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::return_expr> return_node = make_unique<ast::return_expr>();

		eat(lexical::token_type::RETURN_KEYWORD);
		
		return_node->return_value = move(parse_expression());

		return_node->type = ast::expr_type::RETURN_EXPR;

		return return_node;
	}

	auto parser::parse_special_loop_statement() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> node;

		switch (lookahead.type)
		{
		case lexical::token_type::BREAK_KEYWORD:
			node = make_unique<ast::break_expr>();
			node->type = ast::expr_type::BREAK_EXPR;
			break;
		case lexical::token_type::CONTINUE_KEYWORD:
			node = make_unique<ast::continue_expr>();
			node->type = ast::expr_type::CONTINUE_EXPR;
			break;
		default:
			throw exception("unknown loop statement");
			break;
		}
		eat(lookahead.type);
		return node;
	}

	auto parser::parse_expression() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> left = parse_unary_expr();
		
		if (!left) return nullptr;
		
		left = parse_binary_expression(0, left);

		return left;
	}

	auto parser::parse_binary_expression(int expr_prec, std::unique_ptr<ast::expr>& left) -> std::unique_ptr<ast::expr>
	{
		while (true)
		{
			int token_prec = get_token_precedence(lookahead.type);

			if (token_prec < expr_prec) return move(left);

			lexical::token_type _operator = lookahead.type;
			eat(lookahead.type);
			unique_ptr<ast::expr> right = parse_unary_expr();

			if (!right) return nullptr;

			int next_prec = get_token_precedence(lookahead.type);

			if (token_prec < next_prec)
			{
				right = parse_binary_expression(token_prec + 1, right);
				if (!right)
					return nullptr;
			}
			
			unique_ptr<ast::binary_expr> temp = make_unique<ast::binary_expr>();
			
			temp->left = move(left);
			temp->_operator = _operator;
			temp->right = move(right);
			temp->type = ast::expr_type::BINARY_EXPR;
			left = move(temp);
		}
	}

	auto parser::parse_primary_expression() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> node;

		switch (lookahead.type)
		{
		case (lexical::token_type)'(':
			eat((lexical::token_type)'(');
			node = move(parse_expression());
			eat((lexical::token_type)')');
			break;

		case lexical::token_type::STRING_LITERAL:
			node = move(parse_string_literal());
			break;

		case lexical::token_type::FLOAT_LITERAL:
			node = move(parse_float_literal());
			break;
		case lexical::token_type::INTEGER_LITERAL:
			node = move(parse_integer_literal());
			break;

		case lexical::token_type::BOOL_LITERAL:
			node = move(parse_bool_literal());
			break;

		case (lexical::token_type)'{':
			node = move(parse_object_literal());
			break;

		case (lexical::token_type)'[':
			node = move(parse_array_literal());
			break;

		case lexical::token_type::IDENTIFIER:
			node = move(parse_identifier());
			break;
		default:
			node = nullptr;
			break;
		}
		
		node = parse_call_and_access(node);

		return node;
	}

	auto parser::parse_unary_expr() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> node;
		
		if (lookahead.type == lexical::token_type::BITWISE_NEG ||
			lookahead.type == lexical::token_type::LOGICAL_NEG || 
			lookahead.type == lexical::token_type::ADD || 
			lookahead.type == lexical::token_type::SUB)
		{
			unique_ptr<ast::unary_expr> unary_node;

			unary_node = std::make_unique<ast::unary_expr>();
			unary_node->_operator = eat(lookahead.type).type;
			unary_node->operand = move(parse_unary_expr());
			unary_node->type = ast::expr_type::UNARY_EXPR;
			node = move(unary_node);
		}
		else
		{
			node = move(parse_primary_expression());
		}

		return node;
	}

	auto parser::parse_member_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::member_access_expr> node = make_unique<ast::member_access_expr>();
		
		node->object = move(object);
		eat((lexical::token_type)'.');
		node->memeber_identifier = eat(lexical::token_type::IDENTIFIER).lexem;
		node->type = ast::expr_type::MEMBER_ACCESS_EXPR;

		return node;
	}

	auto parser::parse_call_and_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::expr> node = move(object);
		bool running = true;

		while (running)
		{
			switch (lookahead.type)
			{
			case (lexical::token_type)'.':
				node = move(parse_member_access(node));
				break;
			case (lexical::token_type)'[':
				node = move(parse_index_access(node));
				break;
			case (lexical::token_type)'(':
				node = move(parse_call_expression(node));
				break;
			default:
				running = false;
				break;
			}
		}

		return node;
	}

	auto parser::parse_index_access(unique_ptr<ast::expr>& object) -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::index_access_expr> node = make_unique<ast::index_access_expr>();
		
		node->object = move(object);
		eat((lexical::token_type)'[');
		node->index = move(parse_expression());
		eat((lexical::token_type)']');
		
		node->type = ast::expr_type::INDEX_ACCESS_EXPR;

		return node;
	}

	auto parser::parse_call_expression(unique_ptr<ast::expr>& callee) -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::call_expr> node = make_unique<ast::call_expr>();

		node->callee = move(callee);
		eat((lexical::token_type)'(');
		
		if (lookahead.type != (lexical::token_type)')')
			node->args.emplace_back(move(parse_expression()));
		
		while (lookahead.type != (lexical::token_type)')')
		{
			eat((lexical::token_type)',');
			node->args.emplace_back(move(parse_expression()));
		}
		
		eat((lexical::token_type)')');

		node->type = ast::expr_type::CALL_EXPR;

		return node;
	}
	

	auto parser::parse_identifier() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::identifier_expr> node = make_unique<ast::identifier_expr>();

		node->identifier = eat(lexical::token_type::IDENTIFIER).lexem;
		interpeter_assert(data_type_from_string(node->identifier) == (data_type)-1, ("'" + node->identifier + "' cant be an identifier").c_str(), SYNTAX_ERROR);
		node->type = ast::expr_type::IDENTIFIER_EXPR;

		return node;
	}

	auto parser::parse_float_literal() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::float_literal_expr> node = make_unique<ast::float_literal_expr>();
		int8_t sign = 1;

		if (lookahead.type == lexical::token_type::SUB) // - sign
		{
			eat(lexical::token_type::SUB);
			sign = -1;
		}
		else if (lookahead.type == lexical::token_type::ADD) // + sign
		{
			eat(lexical::token_type::ADD);
			sign = 1;
		}

		node->value = sign * stold(eat(lexical::token_type::FLOAT_LITERAL).lexem);
		node->type = ast::expr_type::FLOAT_LITERAL_EXPR;

		return node;
	}

	auto parser::parse_integer_literal() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::integer_literal_expr> node = make_unique<ast::integer_literal_expr>();
		int8_t sign = 1;

		if (lookahead.type == lexical::token_type::SUB) // - sign
		{
			eat(lexical::token_type::SUB);
			sign = -1;
		}
		else if (lookahead.type == lexical::token_type::ADD) // + sign
		{
			eat(lexical::token_type::ADD);
			sign = 1;
		}

		node->value = sign * stoll(eat(lexical::token_type::INTEGER_LITERAL).lexem);
		node->type = ast::expr_type::INTEGER_LITERAL_EXPR;

		return node;
	}
	auto parser::parse_string_literal() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::string_literal_expr> node = make_unique<ast::string_literal_expr>();

		string value = eat(lexical::token_type::STRING_LITERAL).lexem;
		value = value.substr(1, value.size() - 2);
		value = utils::str::escape(value);
		
		node->value = value;
		node->type = ast::expr_type::STRING_LITERAL_EXPR;

		return node;
	}

	auto parser::parse_bool_literal() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::bool_literal_expr> node = make_unique<ast::bool_literal_expr>();

		node->value = eat(lexical::token_type::BOOL_LITERAL).lexem == "true";
		node->type = ast::expr_type::BOOL_LITERAL_EXPR;

		return node;
	}

	auto parser::parse_array_literal() -> unique_ptr<ast::expr>
	{
		unique_ptr<ast::array_literal_expr> node = make_unique<ast::array_literal_expr>();

		eat((lexical::token_type)'[');


		if (lookahead.type != (lexical::token_type)']') 
		{
			node->values.emplace_back(move(parse_expression()));
		}
	
		while (lookahead.type != (lexical::token_type)']')
		{
			eat((lexical::token_type)',');
			node->values.emplace_back(move(parse_expression()));
		}

		eat((lexical::token_type)']');

		node->type = ast::expr_type::ARRAY_LITERAL_EXPR;

		return node;
	}

	auto parser::parse_object_literal() -> unique_ptr<ast::expr>
	{
		pair<unique_ptr<ast::expr>, unique_ptr<ast::expr>> p;
		
		unique_ptr<ast::object_literal_expr> node = make_unique<ast::object_literal_expr>();

		eat((lexical::token_type)'{');

		if (lookahead.type != (lexical::token_type)'}')
		{
			p.first = move(parse_expression());
			eat((lexical::token_type)':');
			p.second = move(parse_expression());

			node->pairs.emplace_back(move(p.first), move(p.second));
		}

		while (lookahead.type != (lexical::token_type)'}')
		{
			eat((lexical::token_type)',');

			p.first = move(parse_expression());
			eat((lexical::token_type)':');
			p.second = move(parse_expression());

			node->pairs.emplace_back(move(p.first), move(p.second));
		}

		eat((lexical::token_type)'}');

		node->type = ast::expr_type::OBJECT_LITERAL_EXPR;
		std::unique_ptr<ast::expr> temp = move(node);
		return temp;
	}

	auto parser::eat(lexical::token_type token_type) -> lexical::token
	{
		if (lookahead.type != token_type)
		{
			switch (token_type)
			{
			case (lexical::token_type)';':
				throw interpeter_error(("expected ; in end of line " + to_string(tokenizer.current_line() - 1)).c_str(), SYNTAX_ERROR);
				break;
			default:
				throw interpeter_error(("unexpected token! at line: " + to_string(lookahead.line)).c_str(), SYNTAX_ERROR);
				break;
			}
		}	
			
		const lexical::token token = lookahead;
		
		tokenizer >> lookahead;

		return token;
	}

	auto parser::get_token_precedence(lexical::token_type token_type) -> int
	{
		int res;

		switch (token_type)
		{
		case lexical::token_type::MULT:
		case lexical::token_type::DIV:
		case lexical::token_type::MODULO:
			res = 110;
			break;
		case lexical::token_type::ADD:
		case lexical::token_type::SUB:
			res = 100;
			break;
		case lexical::token_type::SHIFT_LEFT:
		case lexical::token_type::SHIFT_RIGHT:
			res = 90;
			break;
		case lexical::token_type::LESSER:
		case lexical::token_type::BIGGER:
		case lexical::token_type::LESSER_EQUALS:
		case lexical::token_type::BIGGER_EQUALS:
			res = 80;
			break;
		case lexical::token_type::EQUAL:
		case lexical::token_type::NEQUAL:
			res = 70;
			break;
		case lexical::token_type::BITWISE_AND:
			res = 60;
			break;
		case lexical::token_type::XOR:
			res = 50;
			break;
		case lexical::token_type::BITWISE_OR:
			res = 40;
			break;
		case lexical::token_type::LOGICAL_AND:
			res = 30;
			break;
		case lexical::token_type::LOGICAL_OR:
			res = 20;
			break;
		case lexical::token_type::ASSIGN:
			res = 10;
			break;
		default:
			res = -1;
			break;
		}

		return res;
	}
}