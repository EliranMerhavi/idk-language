#pragma once

#include "abstract_var.h"

/// <summary>
/// used for creating a variable
/// </summary>
/// <param name="d_type">data type</param>
/// <param name="v_type">variable type</param>
/// <param name="value">the value of the variable</param>
/// <returns></returns>
auto init_var(data_type d_type, var_type v_type, const std::shared_ptr<abstract_var>& value=nullptr) -> std::shared_ptr<abstract_var>;
