#pragma once

#include "abstract_var.h"

class bool_var;

class array_var : public abstract_var
{
	using members_t = std::unordered_map<std::string, std::shared_ptr<abstract_var>>;
public:
	using array_t = std::vector<std::shared_ptr<abstract_var>>;

	array_var(const array_t& value, var_type v_type=var_type::CONST);

	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void override;
	virtual auto operator+(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator*(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator<(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator>(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator<=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator>=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator[](const std::shared_ptr<abstract_var>& index) const->std::shared_ptr<abstract_var> override;
	virtual auto get_member(const std::string& identifier) const->std::shared_ptr<abstract_var> override;
	virtual auto is_indexable() const->std::shared_ptr<abstract_var> override;
	virtual operator std::string() const override;
	virtual operator bool() const override;
	virtual auto hash() const->size_t override;
private:
	auto to_index_val(const std::shared_ptr<abstract_var>& index_var) const->size_t;
private:
	array_t value;
	members_t members;

	friend class integer_var;
	friend class bool_var;
	friend class string_var;
	friend class float_var;

	static auto mult(const array_var& arr_var, const bool_var& mult_var) -> std::shared_ptr<abstract_var>;
	static auto mult(const array_var& arr_var, const integer_var& mult_var)->std::shared_ptr<abstract_var>;
};