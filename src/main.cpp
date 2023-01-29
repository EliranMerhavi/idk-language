#include "pch.h"


#include "syntax/parser.h"
#include "lexical/tokenizer.h"
#include "execution/executer.h"

void interpeter_mode();

auto main(int argc, char** argv) -> int
{
	srand(time(NULL));
	if (argc == 1)
	{
		execution::executer executer;
		executer.execute_file("res/test.idk");
		//interpeter_mode();
	}
	else if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		std::cout << "this is -h\n";
	}
	else if (argc == 3 && !strcmp(argv[1], "-e"))
	{
		const std::string filepath = argv[2];
		execution::executer().execute_file(filepath);
	}
	else
	{
		cout << "unknown flags!\nuse -h flag for help\n";
		return 1;
	}
	
	return 0;
}

void interpeter_mode()
{

}