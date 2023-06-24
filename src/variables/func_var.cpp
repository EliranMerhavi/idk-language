#include "pch.h"
#include "func_var.h"
#include "bool_var.h"
#include "string_var.h"
#include "void_var.h"

size_t func_var::function_counter = 0;

func_var::func_var(const ast::function_declaration_expr* value, var_type v_type)
    :
    abstract_var(v_type, data_type::FUNCTION),
    value(),
    args_size(args_size),
    id(function_counter++),
    is_built_in(false)
{
    if (value)
    {
        this->value = [this, value](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
            std::unordered_map<std::string, std::shared_ptr<abstract_var>> args_with_names;

            for (size_t i = 0; i < args.size(); i++)
            {
                const ast::variable_declaration_expr* arg_info = (ast::variable_declaration_expr*)value->args[i].get();
                interpeter_assert(args[i]->get_data_type() == arg_info->d_type, (" at argument " + std::to_string(i) + " at '" + value->identifier +
                    "' expected type '" + data_type_to_string(arg_info->d_type) + "', got '" + data_type_to_string(args[i]->get_data_type()) + "' ").c_str(), RUNTIME_ERROR);

                args_with_names[arg_info->identifier] = args[i];
            }

            executer.execute_func(args_with_names, value);

            return executer.get_return_value();
        };
        this->args_size = value->args.size();
        set_identifier(value->identifier);
    }
}

func_var::func_var(func_value_t value, const std::string& identifier, size_t args_size, var_type v_type) :
    abstract_var(v_type, data_type::FUNCTION, identifier),
    value(value),
    args_size(args_size),
    id(function_counter++),
    is_built_in(true)
{
}

auto func_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
    interpeter_assert(is_mutable(),  "cant use '=' with const variable", RUNTIME_ERROR);

    switch (other->get_data_type())
    {
    case data_type::FUNCTION:
    {
        func_var* other_func = static_cast<func_var*>(other.get());
        this->value = other_func->value;
        this->id = other_func->id;
        set_identifier(other_func->get_identifier());
        this->args_size = other_func->args_size;
        this->is_built_in = other_func->is_built_in;
        break;
    }
    default:
        __super::operator=(other);
        break;
    }
}

auto func_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<bool_var> res;
    switch (other->get_data_type())
    {
    case data_type::FUNCTION:
        res = std::make_shared<bool_var>(this->id == static_cast<func_var*>(other.get())->id);
        break;
    default:
        res = std::make_shared<bool_var>(false);
        break;
    }

    return res;
}

auto func_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
    std::shared_ptr<bool_var> res;
    switch (other->get_data_type())
    {
    case data_type::FUNCTION:
        res = std::make_shared<bool_var>(this->id != static_cast<func_var*>(other.get())->id);
        break;
    default:
        res = std::make_shared<bool_var>(true);
        break;
    }
    return res;
}

auto func_var::operator()(execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) const -> std::shared_ptr<abstract_var>
{
    interpeter_assert((this->args_size == INFINITE_ARGS_SIZE) || (args.size() == this->args_size), ("'" + this->get_identifier() + "' has expected " +
        std::to_string(this->args_size) + " arguments, got " + std::to_string(args.size())).c_str(), RUNTIME_ERROR);

    return this->value(executer, args);
}

auto func_var::is_callable() const -> std::shared_ptr<abstract_var>
{
    return std::make_shared<bool_var>(true);
}

func_var::operator std::string() const
{
    std::string info = "<";
    if (is_built_in)
        info += "built-in ";
    info += "func '" + this->get_identifier() + "'>";
    return info;
}

func_var::operator bool() const
{
    return true;
}
