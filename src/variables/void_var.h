#pragma once

#include "abstract_var.h"

class void_var : public abstract_var
{
public:
	static auto instance() -> std::shared_ptr<abstract_var>&;
private:
	void_var();
};