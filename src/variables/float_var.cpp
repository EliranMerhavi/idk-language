#include "pch.h"
#include "float_var.h"
#include "integer_var.h"
#include "bool_var.h"
#include "string_var.h"

float_var::float_var(ldouble_t value, var_type v_type) : 
    abstract_var(v_type, data_type::FLOAT),
    value(value)
{
}

auto float_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
    interpeter_assert(is_mutable(),  "cant use '=' with const variable", RUNTIME_ERROR);


    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        this->value = static_cast<integer_var*>(other.get())->value;
        break;
    case data_type::FLOAT:
        this->value = static_cast<float_var*>(other.get())->value;
        break;
    case data_type::BOOL:
        this->value = static_cast<bool_var*>(other.get())->value;
        break;
    default:
        __super::operator=(other);
        break;
    }
}

auto float_var::operator+(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<float_var>(this->value + static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value + static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<float_var>(this->value + static_cast<bool_var*>(other.get())->value);
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

auto float_var::operator-(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<float_var>(this->value - static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value - static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<float_var>(this->value - static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator-(other);
        break;
    }
    return res;
}

auto float_var::operator*(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<float_var>(this->value * static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value * static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<float_var>(this->value * static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator*(other);
        break;
    }
    return res;
}

auto float_var::operator/(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    interpeter_assert(other->is_not_zero(),  "zero divison error", RUNTIME_ERROR);
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = std::make_shared<float_var>(this->value / static_cast<integer_var*>(other.get())->value);
        break;
    case data_type::FLOAT:
        res = std::make_shared<float_var>(this->value / static_cast<float_var*>(other.get())->value);
        break;
    case data_type::BOOL:
        res = std::make_shared<float_var>(this->value / static_cast<bool_var*>(other.get())->value);
        break;
    default:
        res = __super::operator/(other);
        break;
    }
    return res;
}

auto float_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
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

auto float_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
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

auto float_var::operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
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

auto float_var::operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
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

auto float_var::operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
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

auto float_var::operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
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

float_var::operator std::string() const
{
    return std::to_string(this->value);
}

float_var::operator bool() const
{
    return this->value;
}

float_var::operator ldouble_t() const
{
    return this->value;
}

auto float_var::hash() const -> size_t
{
    return std::hash<ldouble_t>()(this->value);
}

auto float_var::is_not_zero() const -> bool
{
    return (bool)*this;
}

auto float_var::operator-() const -> std::shared_ptr<abstract_var> 
{
    return std::make_shared<float_var>(-this->value);
}

auto float_var::operator+() const -> std::shared_ptr<abstract_var> 
{
    return std::make_shared<float_var>(this->value);
}
