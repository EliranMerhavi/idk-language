#pragma once

#include "typedefs.h"
#include <unordered_map>
#include <string>
#include <memory>

#include "data_type.h"
#include "var_type.h"

namespace execution
{
	class executer;
}

/// <summary>
/// base class for any type of variable
/// note that default operator overloading 
/// is throwing an exception for unsupported operator
/// </summary>
class abstract_var
{
public:
	abstract_var(var_type v_type=var_type::CONST, data_type d_type=data_type::VOID, const std::string& identifier="");


	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void;

	// math operators
	virtual auto operator+(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator-(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator*(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator/(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto modulo(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;

	// bitwise operators
	virtual auto operator^(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator&(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator|(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator<<(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator>>(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator~() const->std::shared_ptr<abstract_var>;


	// logical operators
	auto operator!() const->std::shared_ptr<abstract_var>;
	auto operator&&(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	auto operator||(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;

	// comparison operators
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator<(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator>(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator<=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;
	virtual auto operator>=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var>;

	virtual auto operator()(execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) const->std::shared_ptr<abstract_var>;
	virtual auto operator[](const std::shared_ptr<abstract_var>& index) const->std::shared_ptr<abstract_var>;
	virtual auto get_member(const std::string& identifier) const->std::shared_ptr<abstract_var>;

	virtual auto is_callable() const->std::shared_ptr<abstract_var>;
	virtual auto is_indexable() const->std::shared_ptr<abstract_var>;
	virtual auto hash() const->size_t;

	virtual explicit operator std::string() const;
	virtual explicit operator bool() const;
	virtual explicit operator ldouble_t() const;

	virtual auto operator-() const->std::shared_ptr<abstract_var>;
	virtual auto operator+() const->std::shared_ptr<abstract_var>;

	friend auto operator<<(std::ostream& os, const abstract_var& var)->std::ostream&;

	inline auto is_named_var() -> bool
	{
		return identifier.size();
	}

	inline auto get_data_type() const -> data_type
	{
		return d_type;
	}
	
	inline auto is_mutable() const -> bool
	{
		return v_type != var_type::CONST;
	}
	
	inline auto set_identifier(const std::string& identifier) -> void
	{
		this->identifier = identifier;
	}
	
	inline auto get_identifier() const -> std::string
	{
		return this->identifier;
	}

	/// <summary>
	/// check if *this = 0
	/// </summary>
	/// <returns>true if the variable is equal to the number 0 otherwise false </returns>
	virtual auto is_not_zero() const -> bool;
private:
	data_type d_type;
	var_type v_type;
	std::string identifier;

	friend auto init_var(data_type d_type, var_type v_type, const std::shared_ptr<abstract_var>& value) -> std::shared_ptr<abstract_var>;
};

template<>
struct std::hash<std::shared_ptr<abstract_var>>
{
	size_t operator()(const std::shared_ptr<abstract_var>& var) const
	{
		return var->hash();
	}
};

template<>
struct std::equal_to<std::shared_ptr<abstract_var>>
{
	size_t operator()(const std::shared_ptr<abstract_var>& v1, const std::shared_ptr<abstract_var>& v2) const
	{
		return (bool)*(v1->operator==(v2));
	}
};
