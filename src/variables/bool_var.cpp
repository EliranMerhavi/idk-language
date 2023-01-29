#include "pch.h"
#include "bool_var.h"
#include "integer_var.h"
#include "float_var.h"
#include "string_var.h"
#include "array_var.h"

bool_var::bool_var(bool value, var_type v_type) : 
    abstract_var(v_type, data_type::BOOL),
    value(value)
{
}

auto bool_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
    interpeter_assert(is_mutable(),  "cant use '=' with const variable", RUNTIME_ERROR);
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        this->value = (bool)*static_cast<integer_var*>(other.get());
        break;
    case data_type::FLOAT:
        this->value = (bool)*static_cast<float_var*>(other.get());
        break;
    case data_type::BOOL:
        this->value = (bool)*static_cast<bool_var*>(other.get());
        break;
    case data_type::STRING:
        this->value = (bool)*static_cast<string_var*>(other.get());
        break;
    default:
        __super::operator=(other);
        break;
    }
}

auto bool_var::operator+(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;

    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value + static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value + static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value + static_cast<bool_var*>(other.get())->value);
        break;
    case data_type::STRING:
        res = std::make_shared<string_var>(std::to_string(this->value) + static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = __super::operator+(other);
        break;
    }
    return res;
}

auto bool_var::operator-(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value - static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value - static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value - static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator-(other);
        break;
    }
    return res;
}

auto bool_var::operator*(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value * static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value * static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value * static_cast<bool_var*>(other.get())->value);
        break;
    case data_type::ARRAY:
        res = array_var::mult(*static_cast<array_var*>(other.get()), *this);
        break;
    case data_type::STRING:
        res = string_var::mult(*static_cast<string_var*>(other.get()), *this);
        break;
    default:
        res = __super::operator*(other);
        break;
    }
    return res;
}

auto bool_var::operator/(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    interpeter_assert(other->is_not_zero(),  "zero divison error", RUNTIME_ERROR);
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value / static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value / static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value / static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator/(other);
        break;
    }
    return res;
}

auto bool_var::modulo(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    interpeter_assert(other->is_not_zero(),  "zero modulo error", RUNTIME_ERROR);
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value % static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value % static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator<(other);
        break;
    }

    return res;
}

auto bool_var::operator^(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value ^ static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value ^ static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator^(other);
        break;
    }
    return res;
}

auto bool_var::operator&(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value & static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value & static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator&(other);
        break;
    }
    return res;
}

auto bool_var::operator|(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value | static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value | static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator|(other);
        break;
    }
    return res;
}

auto bool_var::operator<<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value << static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value << static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator<<(other);
        break;
    }
    return res;
}

auto bool_var::operator>>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<integer_var>(this->value >> static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<integer_var>(this->value >> static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>>(other);
        break;
    }
    return res;
}

auto bool_var::operator~() const -> std::shared_ptr<abstract_var>
{
    return std::make_shared<integer_var>(~this->value);
}

auto bool_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<bool_var>(this->value == static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<bool_var>(this->value == static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<bool_var>(this->value == static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = std::make_shared<bool_var>(false);
        break;
    }
    return res;
}

auto bool_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<bool_var>(this->value != static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<bool_var>(this->value != static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<bool_var>(this->value != static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = std::make_shared<bool_var>(true);
        break;
    }
    return res;
}

auto bool_var::operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<bool_var>(this->value < static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<bool_var>(this->value < static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<bool_var>(this->value < static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator<(other);
        break;
    }
    return res;
}

auto bool_var::operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<bool_var>(this->value > static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<bool_var>(this->value > static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<bool_var>(this->value > static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>(other);
        break;
    }

    return res;
}

auto bool_var::operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<bool_var>(this->value <= static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<bool_var>(this->value <= static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<bool_var>(this->value <= static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator<=(other);
        break;
    }
    return res;
}

auto bool_var::operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<bool_var>(this->value >= static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<bool_var>(this->value >= static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<bool_var>(this->value >= static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>=(other);
        break;
    }
    return res;
}

auto bool_var::operator-() const -> std::shared_ptr<abstract_var> 
{
    return std::make_shared<bool_var>(-this->value);
}

auto bool_var::operator+() const -> std::shared_ptr<abstract_var>
{
    return std::make_shared<bool_var>(this->value);
}

bool_var::operator std::string() const
{
    return this->value ? "true" : "false";
}

bool_var::operator bool() const
{
    return this->value;
}

bool_var::operator ldouble_t() const
{
    return this->value;
}

auto bool_var::hash() const -> size_t
{
    return std::hash<bool>()(this->value);
}

auto bool_var::is_not_zero() const -> bool 
{
    return (bool)*this;
}
