#pragma once

#include "abstract_var.h"

class integer_var : public abstract_var
{
public:
	integer_var(int64_t value=0, var_type v_type=var_type::CONST);
	
	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void override;
	virtual auto operator+(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator-(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator*(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator/(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto modulo(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator^(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator&(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator|(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator<<(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator>>(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator~() const->std::shared_ptr<abstract_var> override;
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;
	virtual auto operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> override;

	virtual auto operator-() const->std::shared_ptr<abstract_var> override;
	virtual auto operator+() const->std::shared_ptr<abstract_var> override;

	virtual operator std::string() const override;
	virtual operator bool() const override;
	virtual operator ldouble_t() const override;
	virtual auto hash() const->size_t override;
	virtual auto is_not_zero() const -> bool override;

private:
	int64_t value;

	friend class bool_var;
	friend class string_var;
	friend class float_var;
	friend class array_var;
};
