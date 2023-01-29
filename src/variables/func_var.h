#pragma once

#include <functional>
#include "abstract_var.h"
#include "execution/executer.h"
#include "syntax/ast.h"

#define INFINITE_ARGS_SIZE UINT64_MAX
class func_var : public abstract_var
{
	using func_value_raw_t = std::shared_ptr<abstract_var>(*)(execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args);
	using func_value_t = std::function<std::shared_ptr<abstract_var>(execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args)>;
public:
	func_var(const ast::function_declaration_expr* value, var_type v_type = var_type::CONST);
	func_var(func_value_t value, const std::string& identifier, size_t args_size, var_type v_type = var_type::CONST);

	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void override;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator()(execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) const->std::shared_ptr<abstract_var> override;
	virtual auto is_callable() const->std::shared_ptr<abstract_var> override;

	virtual operator std::string() const override;
	virtual operator bool() const override;
private:
	func_value_t value;
	size_t id;
	static size_t function_counter;
	size_t args_size;
	bool is_built_in;
};