#pragma once
#include "abstract_var.h"

/// <summary>
/// this class represent the float data type
/// </summary>
class float_var : public abstract_var
{
public:
	float_var(ldouble_t value = 0.0f, var_type v_type = var_type::CONST);
	
	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void override;
	
	virtual auto operator+(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator-(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator*(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator/(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual operator std::string() const override;
	virtual operator bool() const override;
	virtual operator ldouble_t() const override;
	virtual auto hash() const->size_t override;
	virtual auto is_not_zero() const -> bool override;

	virtual auto operator-() const->std::shared_ptr<abstract_var> override;
	virtual auto operator+() const->std::shared_ptr<abstract_var> override;
private:
	ldouble_t value;

	friend class integer_var;
	friend class bool_var;
	friend class string_var;
};