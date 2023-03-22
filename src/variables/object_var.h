#pragma once

#include "abstract_var.h"

/// <summary>
/// this class represent the object data type
/// </summary>
class object_var : public abstract_var
{
	using members_t = std::unordered_map<std::string, std::shared_ptr<abstract_var>>;
public:
	using map_t = std::unordered_map<std::shared_ptr<abstract_var>, std::shared_ptr<abstract_var>>;

	object_var(const map_t& value, var_type v_type=var_type::CONST);
	
	virtual auto operator=(const std::shared_ptr<abstract_var>& other) -> void override;
	virtual auto operator+(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator-(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator==(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator!=(const std::shared_ptr<abstract_var>& other) const->std::shared_ptr<abstract_var> override;
	virtual auto operator[](const std::shared_ptr<abstract_var>& index) const->std::shared_ptr<abstract_var> override;
	virtual auto get_member(const std::string& identifier) const->std::shared_ptr<abstract_var> override;
	virtual auto is_indexable() const->std::shared_ptr<abstract_var> override;

	virtual explicit operator std::string() const;
	virtual explicit operator bool() const;
	virtual auto hash() const->size_t override;
private:
	map_t value;
	members_t members;
};

