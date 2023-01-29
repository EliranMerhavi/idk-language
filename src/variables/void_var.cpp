#include "pch.h"
#include "void_var.h"
#include "string_var.h"
#include "bool_var.h"

void_var::void_var() : abstract_var()
{
}

auto void_var::instance() -> std::shared_ptr<abstract_var>&
{
	static std::shared_ptr<abstract_var> _instance(new void_var);
	
	return _instance;
}