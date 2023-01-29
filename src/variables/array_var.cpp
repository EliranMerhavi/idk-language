#include "pch.h"
#include "array_var.h"
#include "string_var.h"
#include "bool_var.h"
#include "float_var.h"
#include "integer_var.h"
#include "func_var.h"
#include "void_var.h"
#include "var_utils.h"

array_var::array_var(const array_t& value, var_type v_type) :
    abstract_var(v_type, data_type::ARRAY),
    value(value.size())
{
    for (int i = 0; i < value.size(); i++)
    {
        const auto& element = value[i];
        this->value[i] = init_var(element->get_data_type(), var_type::LET, element);
    }

    this->members["length"] = std::make_shared<func_var>(
        [=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
            return std::make_shared<integer_var>(this->value.size());
        }, "array.append", 0);

    this->members["append"] = std::make_shared<func_var>(
        [=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
            this->value.push_back(args.front());
            return void_var::instance();
        }, "array.append", 1);

    this->members["remove_at"] = std::make_shared<func_var>(
        [=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {

            size_t index_val = to_index_val(args.front());
            std::shared_ptr<abstract_var> res = this->value[index_val];
            this->value.erase(std::next(this->value.begin(), index_val));

            return void_var::instance();
        }, "array.remove_at", 1);
}

auto array_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
    interpeter_assert(is_mutable(),  "cant use '=' with const variable", RUNTIME_ERROR);
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
    {
        this->value.clear();
        const array_t& other_value = static_cast<array_var*>(other.get())->value;
        
        std::move(
            other_value.begin(),
            other_value.end(),
            std::back_inserter(this->value)
        );
        break;
    }
    default:
        __super::operator=(other);
        break;
    }
}

auto array_var::operator+(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
    {
        array_t res_value, 
            other_value = static_cast<array_var*>(other.get())->value;
        
        std::move(
            this->value.begin(),
            this->value.end(),
            std::back_inserter(res_value)
        );

        std::move(
            other_value.begin(),
            other_value.end(),
            std::back_inserter(res_value)
        );

        res = std::make_shared<array_var>(res_value);
        break;
    } 
    default:
        res = __super::operator+(other);
        break;
    }

    return res;
}

auto array_var::operator*(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    
    switch (other->get_data_type())
    {
    case data_type::INTEGER:
        res = array_var::mult(*this, *static_cast<integer_var*>(other.get()));
        break;
    case data_type::BOOL:
        res = array_var::mult(*this, *static_cast<bool_var*>(other.get()));
        break;
    default:
        res = __super::operator*(other);
        break;
    }

    return res;
}

auto array_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
        res = std::make_shared<bool_var>(this->value == static_cast<array_var*>(other.get())->value);
        break;
    default:
        res = std::make_shared<bool_var>(false);
        break;
    }
    return res;
}

auto array_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
        res = std::make_shared<bool_var>(this->value != static_cast<array_var*>(other.get())->value);
        break;
    default:
        res = std::make_shared<bool_var>(true);
        break;
    }
    return res;
}

auto array_var::operator<(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
        res = std::make_shared<bool_var>(this->value < static_cast<array_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>=(other);
        break;
    }
    return res;
}

auto array_var::operator>(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
        res = std::make_shared<bool_var>(this->value > static_cast<array_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>=(other);
        break;
    }
    return res;
}

auto array_var::operator<=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
        res = std::make_shared<bool_var>(this->value <= static_cast<array_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>=(other);
        break;
    }
    return res;
}

auto array_var::operator>=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    switch (other->get_data_type())
    {
    case data_type::ARRAY:
        res = std::make_shared<bool_var>(this->value >= static_cast<array_var*>(other.get())->value);
        break;
    default:
        res = __super::operator>=(other);
        break;
    }
    return res;
}


auto array_var::operator[](const std::shared_ptr<abstract_var>& index) const -> std::shared_ptr<abstract_var>
{
    return this->value[to_index_val(index)];
}

auto array_var::get_member(const std::string& identifier) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<abstract_var> res;
    if (members.find(identifier) != members.end())
    {
        res = members.at(identifier);
    }
    else
    {
        res = __super::get_member(identifier);
    }

    return res;
}

auto array_var::is_indexable() const -> std::shared_ptr<abstract_var>
{
    return std::make_shared<bool_var>(true);
}

array_var::operator std::string() const
{
    std::string string_value;

    if (this->value.size())
    {
        string_value = "[";
        for (int i = 0; i < this->value.size() - 1; i++)
        {
                string_value += (this->value[i].get() != this) ? 
                    (std::string)*this->value[i] + ", " : 
                    "<inner-reference>, ";
        }
    
        string_value += (this->value.back().get() != this) ? (std::string)*this->value.back() + "]" :
            "<inner-reference>]";
    }
    else
    {
        string_value = "[]";
    }
    
    return string_value;
}

array_var::operator bool() const
{
    return this->value.size();
}

auto array_var::hash() const -> size_t
{
    size_t seed = this->value.size();
    for (const auto& element : this->value)
        seed ^= element->hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}

auto array_var::mult(const array_var& arr_var, const bool_var& mult_var) -> std::shared_ptr<abstract_var>
{
    return std::make_shared<array_var>(mult_var.value ? arr_var.value : array_t());
}

auto array_var::mult(const array_var& arr_var, const integer_var& mult_var) -> std::shared_ptr<abstract_var>
{
    array_t res;

    for (int i = 0; i < mult_var.value; i++)
    {
        std::move(
            arr_var.value.begin(),
            arr_var.value.end(),
            std::back_inserter(res)
        );
    }

    return std::make_shared<array_var>(res);
}

auto array_var::to_index_val(const std::shared_ptr<abstract_var>& index_var) const -> size_t
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
