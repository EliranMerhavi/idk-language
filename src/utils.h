
#include <string>
#include <vector>

namespace utils
{
	/// <summary>
	/// used for loading the content of a file 
	/// as a list of lines (with ' ' instead of '\n' at the end (helps at lexical phase))
	/// </summary>
	/// <param name="filepath">filepath</param>
	/// <returns>list of lines</returns>
	auto load_source(const std::string& filepath) -> std::vector<std::string>;
	namespace str
	{
		/// <summary>
		/// used for escaping chars in a string
		/// example of escaption: for each 2 chars that 
		///						  are equal to "\\n" convert it to '\n'.
		/// </summary>
		/// <param name="str">the string to escape</param>
		/// <returns>the escaped string</returns>
		auto escape(const std::string& str)->std::string;
		auto count(const std::string& str, char ch)->size_t;
	}
}