#include "pch.h"
#include "var_utils.h"
#include "string_var.h"
#include "integer_var.h"
#include "bool_var.h"
#include "float_var.h"
#include "func_var.h"
#include "array_var.h"
#include "object_var.h"
auto init_var(data_type d_type, var_type v_type, const std::shared_ptr<abstract_var>& value) -> std::shared_ptr<abstract_var>
{
	std::shared_ptr<abstract_var> var;
	switch (d_type)
	{
	case data_type::INTEGER:
		var = std::make_shared<integer_var>(0, var_type::LET);
		break;
	case data_type::FLOAT:
		var = std::make_shared<float_var>(0, var_type::LET);
		break;
	case data_type::STRING:
		var = std::make_shared<string_var>("", var_type::LET);
		break;
	case data_type::OBJECT:
		var = std::make_shared<object_var>(std::unordered_map<std::shared_ptr<abstract_var>, std::shared_ptr<abstract_var>>(), var_type::LET);
		break;
	case data_type::BOOL:
		var = std::make_shared<bool_var>(false, var_type::LET);
		break;
	case data_type::ARRAY:
		var = std::make_shared<array_var>(std::vector<std::shared_ptr<abstract_var>>(), var_type::LET);
		break;
	case data_type::FUNCTION:
		var = std::make_shared<func_var>(nullptr, var_type::LET);
		break;
	case data_type::VOID:
		throw std::exception("'void' cant be a variable");
		break;
	default:
		throw std::exception("unknown data type");
		break;
	}
	
	if (value)
		*var = value;
	
	var->v_type = v_type;
	return var;
}

