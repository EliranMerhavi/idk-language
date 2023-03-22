#include "pch.h"

interpeter_error::interpeter_error(const std::string& msg, error_t type) : 
    msg((std::string("[") + error_t_to_string(type) + "]\n[error] " + msg)), type(type)
{
}
