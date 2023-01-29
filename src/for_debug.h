#pragma once

#include "syntax/ast.h"
namespace debug
{
	void print_tree(const unique_ptr<ast::expr> &root, int indent=4, int depth=0, bool new_line=false);
}