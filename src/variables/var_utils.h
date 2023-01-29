#pragma once

#include "string_var.h"
#include "integer_var.h"
#include "bool_var.h"
#include "float_var.h"
#include "func_var.h"
#include "array_var.h"
#include "object_var.h"

auto init_var(data_type d_type, var_type v_type, const std::shared_ptr<abstract_var>& value=nullptr) -> std::shared_ptr<abstract_var>;
