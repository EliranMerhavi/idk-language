#pragma once

#include "abstract_var.h"

/// <summary>
/// this singleton class represent the void data type
/// </summary>
class void_var : public abstract_var
{
public:
	static auto instance() -> std::shared_ptr<abstract_var>&;
private:
	void_var();
};