#include "pch.h"


#include "syntax/parser.h"
#include "lexical/tokenizer.h"
#include "execution/executer.h"


/// <summary>
/// main
/// </summary>
/// <param name="argc">size of argv</param>
/// <param name="argv">arguments</param>
/// <returns></returns>
auto main(int argc, char** argv) -> int
{
	srand(time(NULL));
	if (argc == 1) // currently its a testing mode
	{
		std::cout << "[ENTERED TESTING MODE]\n";
		execution::executer executer;
		executer.execute_file("res/test.idk");
		//interpeter_mode();
	}
	else if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		std::cout << 
			"[ENTERED HELP MODE]\n"
			"-e <filepath>: for executing a content of a file\n"
			"-h: for help\n";
	}
	else if (argc == 3 && !strcmp(argv[1], "-e"))
	{
		const std::string filepath = argv[2];
		execution::executer().execute_file(filepath);
	}
	else
	{
		std::cout << "unknown flags!\nuse -h flag for help\n";
		return 1;
	}
	
	return 0;
}
