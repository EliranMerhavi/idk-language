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
}