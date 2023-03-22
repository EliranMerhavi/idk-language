#include "pch.h"
#include "string_var.h"
#include "bool_var.h"
#include "integer_var.h"
#include "float_var.h"
#include "array_var.h"
#include "func_var.h"

string_var::string_var(const std::string& value, var_type v_type) : 
    abstract_var(v_type, data_type::STRING),
    value(value)
{
    this->members["length"] = std::make_shared<func_var>(
        [=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
            return std::make_shared<integer_var>(this->value.size());
        }, "string.append", 0);
}

auto string_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
    interpeter_assert(is_mutable(),  "cant use '=' with const variable", RUNTIME_ERROR);
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
    case data_type::FLOAT:
    case data_type::BOOL:
    case data_type::ARRAY:
    case data_type::OBJECT:
    case data_type::FUNCTION:
    case data_type::STRING:
        this->value = (std::string)*other;
        break;
    default:
        __super::operator=(other);
        break;
    }
}

auto string_var::operator+(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
    case data_type::INTEGER:
    case data_type::FLOAT:
    case data_type::BOOL:
    case data_type::ARRAY:
    case data_type::OBJECT:
    case data_type::FUNCTION:
        res = std::make_shared<string_var>(this->value + (std::string)*other);
        break;
    default:
        res = __super::operator+(other);
        break;
    }
    
    return res;
}

auto string_var::operator*(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;

    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = mult(*this, *static_cast<integer_var*>(other.get()));
        break;
    case data_type::BOOL:
        res = mult(*this, *static_cast<bool_var*>(other.get()));
        break;
    default:
        res = __super::operator*(other);
        break;
    }

    return res;
}

auto string_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
        res = std::make_shared<bool_var>(this->value == static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = std::make_shared<bool_var>(false);
        break;
    }

    return res;
}

auto string_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
        res = std::make_shared<bool_var>(this->value != static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = std::make_shared<bool_var>(true);
        break;
    }
    return res;
}

auto string_var::operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
        res = std::make_shared<bool_var>(this->value < static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = __super::operator<(other);
        break;
    }
    return res;
}

auto string_var::operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
        res = std::make_shared<bool_var>(this->value > static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>(other);
        break;
    }
    return res;
}

auto string_var::operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
        res = std::make_shared<bool_var>(this->value <= static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = __super::operator<=(other);
        break;
    }
    return res;
}

auto string_var::operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::STRING:
        res = std::make_shared<bool_var>(this->value >= static_cast<string_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>=(other);
        break;
    }
    return res;
}

auto string_var::operator[](const std::shared_ptr<abstract_var>& index) const -> std::shared_ptr<abstract_var>
{
    return std::make_shared<string_var>(std::string() + this->value[to_index_val(index)]);
}

auto string_var::get_member(const std::string& identifier) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;

    if (members.find(identifier) != members.end())
        res = members.at(identifier);
    else
        res = __super::get_member(identifier);

    return res;
}

auto string_var::is_indexable() const -> std::shared_ptr<abstract_var>
{
    return std::make_shared<bool_var>(true);
}

string_var::operator bool() const
{
    return value.size();
}

auto string_var::hash() const -> size_t
{
    return std::hash<std::string>()(this->value);
}

string_var::operator std::string() const
{
    return "\"" + this->value + "\"";
}

auto string_var::mult(const string_var& str_var, const integer_var& mult_var) -> std::shared_ptr<abstract_var>
{
    std::string res;

    for (int i = 0; i < mult_var.value; i++)
    {
        res += str_var.value;
    }

    return std::make_shared<string_var>(res);
}

auto string_var::mult(const string_var& str_var, const bool_var& mult_var) -> std::shared_ptr<abstract_var>
{
    return std::make_shared<string_var>(mult_var.value ? str_var.value : "");
}

auto string_var::to_index_val(const std::shared_ptr<abstract_var>& index_var) const -> size_t
{
    size_t index_val;
    switch (index_var->get_data_type())
    {
    case data_type::INTEGER:
        index_val = static_cast<integer_var*>(index_var.get())->value;
        break;
    case data_type::BOOL:
        index_val = static_cast<bool_var*>(index_var.get())->value;
        break;
    default:
        throw std::exception("index value of 'array' must be 'integer' or 'bool'");
        break;
    }

    interpeter_assert(0 <= index_val && index_val < this->value.size(),  "'array' index out of range", RUNTIME_ERROR);
    return index_val;
}