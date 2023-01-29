#include "pch.h"
#include "abstract_var.h"
#include "string_var.h"
#include "bool_var.h"
#include "integer_var.h"
#include "func_var.h"

abstract_var::abstract_var(var_type v_type, data_type d_type, const std::string& identifier) 
	: v_type(v_type), d_type(d_type), identifier(identifier)
{
}

auto operator<<(std::ostream& os, const abstract_var& var) -> std::ostream&
{
	std::string str = (std::string)var;
	return os << ((var.d_type != data_type::STRING) ? str : str.substr(1, str.size() - 2));
}

auto abstract_var::operator!() const -> std::shared_ptr<abstract_var>
{
	return std::make_shared<bool_var>(!((bool)*this));
}

auto abstract_var::operator&&(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	return std::make_shared<bool_var>(*this && *other);
}

auto abstract_var::operator||(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	return std::make_shared<bool_var>(*this || *other);
}

auto abstract_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '==' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '!=' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '<' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '>' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '<=' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '>=' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator()(execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("'" + data_type_to_string(this->get_data_type()) + "' is not callable").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator[](const std::shared_ptr<abstract_var>& index) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("'" + data_type_to_string(this->get_data_type())  + "' is not indexable").c_str(), RUNTIME_ERROR);
}

auto abstract_var::get_member(const std::string& identifier) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("'" + data_type_to_string(this->get_data_type()) + "' doenst have member '" + identifier + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::is_callable() const -> std::shared_ptr<abstract_var>
{
	return std::make_shared<bool_var>(false);
}

auto abstract_var::is_indexable() const -> std::shared_ptr<abstract_var>
{
	return std::make_shared<bool_var>(false);
}

auto abstract_var::hash() const -> size_t
{
	throw interpeter_error(("'" + data_type_to_string(this->get_data_type()) + "' is not hashable").c_str(), RUNTIME_ERROR);
}

abstract_var::operator std::string() const
{
	throw interpeter_error(("'" + data_type_to_string(this->get_data_type()) + "' cant be casted to 'string'").c_str(), RUNTIME_ERROR);
}

abstract_var::operator bool() const
{
	throw interpeter_error(("'" + data_type_to_string(this->get_data_type()) + "' doenst have boolean value").c_str(), RUNTIME_ERROR);
}

abstract_var::operator ldouble_t() const
{
	throw interpeter_error(("cant convert '" + data_type_to_string(this->get_data_type()) + "' to numeric value").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator-() const -> std::shared_ptr<abstract_var>
{
	throw std::exception(("cant use '-' with '" + data_type_to_string(this->get_data_type()) + "'").c_str());
}

auto abstract_var::operator+() const -> std::shared_ptr<abstract_var>
{
	throw std::exception(("cant use '+' with '" + data_type_to_string(this->get_data_type()) + "'").c_str());
}

auto abstract_var::is_not_zero() const -> bool
{
	return true;
}

auto abstract_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
	throw interpeter_error(("cant use '=' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator+(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '+' with '"+data_type_to_string(this->get_data_type())+"' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator-(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '-' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator*(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '*' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator/(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '/' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::modulo(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '%' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator^(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '^' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator&(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '&' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator|(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '|' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator<<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '<<' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator>>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '>>' with '" + data_type_to_string(this->get_data_type()) + "' and '" + data_type_to_string(other->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}

auto abstract_var::operator~() const -> std::shared_ptr<abstract_var>
{
	throw interpeter_error(("cant use '~' with '" + data_type_to_string(this->get_data_type()) + "'").c_str(), RUNTIME_ERROR);
}
