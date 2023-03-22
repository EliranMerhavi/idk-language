#include "pch.h"
#include "utils.h"

namespace utils
{
	auto load_source(const std::string& filepath) -> std::vector<std::string>
	{
		std::ifstream ifs(filepath);
		assert(ifs.is_open(), ("couldnt open file " + filepath).c_str());

		std::vector<std::string> lines;
		std::string line;

		while (std::getline(ifs, line))
			lines.push_back(line);

		return lines;
	}

	auto str::escape(const std::string& str) -> std::string
	{
		std::string res;
		std::string::const_iterator it = str.begin();

		while (it != str.end())
		{
			char c = *it++;
			if (c == '\\')
			{
				switch (*it)
				{
				case '\\':
					c = '\\';
					break;
				case '"':
					c = '\"';
					break;
				case 'n':
					c = '\n';
					break;
				case 't':
					c = '\t';
					break;
				default:
					throw interpeter_error("invalid string value (invalid escape sequence char)", SYNTAX_ERROR);
					break;
				}
				it++;
			}
				
			res += c;
		}

		return res;
	}

	auto str::count(const std::string& str, char ch) -> size_t
	{
		size_t res = 0;
		for (const char& _ch : str)
			if (_ch == ch)
				res++;
		return res;
	}
}


