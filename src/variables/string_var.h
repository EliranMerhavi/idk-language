#pragma once
#include "abstract_var.h"

/// <summary>
/// this class represent the string data type
/// </summary>
class string_var : public abstract_var
{
	using members_t = std::unordered_map<std::string, std::shared_ptr<abstract_var>>;
public:
	string_var(const std::string& value = "", var_type v_type = var_type::CONST);
	
	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void override;
	virtual auto operator+(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator*(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;

	virtual auto operator[](const std::shared_ptr<abstract_var>& index) const->std::shared_ptr<abstract_var> override;
	virtual auto get_member(const std::string& identifier) const->std::shared_ptr<abstract_var> override;
	virtual auto is_indexable() const -> std::shared_ptr<abstract_var> override;

	virtual operator std::string() const override;
	virtual operator bool() const override;
	virtual auto hash() const->size_t override;
private:
	std::string value;
	members_t members;

	friend class integer_var;
	friend class bool_var;
	friend class float_var;
	friend class func_var;
	friend class array_var;
	friend class object_var;

	static auto mult(const string_var& str_var, const integer_var& mult_var) -> std::shared_ptr<abstract_var>;
	static auto mult(const string_var& str_var, const bool_var& mult_var) -> std::shared_ptr<abstract_var>;
	auto to_index_val(const std::shared_ptr<abstract_var>& index_var) const->size_t;

	friend auto operator<<(std::ostream& os, const abstract_var& var)->std::ostream&;
};

