#include "pch.h"
#include "object_var.h"
#include "var_utils.h"

object_var::object_var(const map_t& value, var_type v_type) : 
	abstract_var(v_type, data_type::OBJECT),
	value()
{
	for (const auto& [k, v] : value)
	{
		this->value[k] = init_var(v->get_data_type(), var_type::LET, v);
	}

	this->members["length"] = std::make_shared<func_var>(
		[=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
			return std::make_shared<integer_var>(this->value.size());
		}, "object.append", 0);

	this->members["put"] = std::make_shared<func_var>(
		[=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
			this->value[args[0]] = init_var(args[1]->get_data_type(), var_type::LET, args[1]);
			return void_var::instance();
		}, "object.append", 2);

	this->members["erase"] = std::make_shared<func_var>(
		[=](execution::executer& executer, const std::vector<std::shared_ptr<abstract_var>>& args) {
			this->value.erase(args[0]);
			return void_var::instance();
		}, "object.append", 1);
}

auto object_var::operator=(const std::shared_ptr<abstract_var>& other) -> void
{
	interpeter_assert(is_mutable(),  "cant use '=' with const variable", RUNTIME_ERROR);
	switch (other->get_data_type())
	{
	case data_type::OBJECT:
	{
		const map_t& other_value = static_cast<object_var*>(other.get())->value;
		this->value.clear();
		this->value.insert(other_value.begin(), other_value.end());
		break;
	}
	default:
		__super::operator=(other);
		break;
	}
}

auto object_var::operator+(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var> 
{
	std::shared_ptr<abstract_var> res;
	switch (other->get_data_type())
	{
	case data_type::OBJECT:
	{
		map_t res_value,
			other_value = static_cast<object_var*>(other.get())->value;

		res_value.insert(other_value.begin(), other_value.end());

		res_value.insert(this->value.begin(), this->value.end());

		res = std::make_shared<object_var>(res_value);
		break;
	}
	default:
		res = __super::operator+(other);
		break;
	}

	return res;
}

auto object_var::operator-(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	std::shared_ptr<abstract_var> res;
	switch (other->get_data_type())
	{
	case data_type::OBJECT:
	{
		map_t res_value,
			other_value = static_cast<object_var*>(other.get())->value;

		res_value.insert(this->value.begin(), this->value.end());
		res_value.erase(other_value.begin(), other_value.end());
		res = std::make_shared<object_var>(res_value);
		break;
	}
	default:
		res = __super::operator-(other);
		break;
	}
	return res;
}

auto object_var::operator==(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	std::shared_ptr<abstract_var> res;
	switch (other->get_data_type())
	{
	case data_type::OBJECT:
	{	
		res = std::make_shared<bool_var>(this->value == static_cast<object_var*>(other.get())->value);
		break;
	}
	default:
		res = std::make_shared<bool_var>(true);
		break;
	}
	return res;
}

auto object_var::operator!=(const std::shared_ptr<abstract_var>& other) const -> std::shared_ptr<abstract_var>
{
	std::shared_ptr<abstract_var> res;
	switch (other->get_data_type())
	{
	case data_type::OBJECT:
	{
		res = std::make_shared<bool_var>(this->value != static_cast<object_var*>(other.get())->value);
		break;
	}
	default:
		res = std::make_shared<bool_var>(true);
		break;
	}
	return res;
}

auto object_var::operator[](const std::shared_ptr<abstract_var>& index) const -> std::shared_ptr<abstract_var>
{
	std::shared_ptr<abstract_var> res;
	const auto& it = value.find(index);
	if (it != value.end())
	{
		res = it->second;
	}
	else
	{
		throw std::exception(("index " + (std::string)*index + " doenst exists in this object").c_str());
	}
	return res;
}

auto object_var::get_member(const std::string& identifier) const -> std::shared_ptr<abstract_var>
{
	std::shared_ptr<abstract_var> res;
	if (members.find(identifier) != members.end())
	{
		res = members.at(identifier);
	}
	else
	{
		const auto& it = this->value.find(std::make_shared<string_var>(identifier));

		if (it != this->value.end())
		{
			res = it->second;
		}
		else
		{
			res = __super::get_member(identifier);
		}
	}
	
	return res;
}

auto object_var::is_indexable() const -> std::shared_ptr<abstract_var>
{
	return std::make_shared<bool_var>(true);
}

object_var::operator bool() const
{
	return this->value.size();
}

object_var::operator std::string() const
{
	std::string res;
	if (!this->value.empty())
	{
		res += "{";
		for (const auto& [key, value] : this->value)
		{
			res += (std::string)*key + ": " + (std::string)*value + ", ";
		}
		res = res.substr(0, res.size() - 2) + "}";
	}
	else
	{
		res = "{}";
	}
	return res;
}

auto object_var::hash() const -> size_t
{
	size_t seed = this->value.size();
	for (const auto& [key, value] : this->value)
		seed ^= key->hash() + value->hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	return seed;
}


