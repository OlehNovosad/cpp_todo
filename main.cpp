#include "inc/todo.h"
#include <iostream>
#include <string>

void print_usage(const char *prog)
{
	std::cerr << "Usage:\n"
			  << "  " << prog << " add <task>\n"
			  << "  " << prog << " done <task_id>\n"
			  << "  " << prog << " delete <task_id>\n";
}

int main(const int argc, char **argv)
{
	if (argc < 2)
	{
		print_usage(argv[0]);
		return 1;
	}

	ToDo todo;

	todo.add("Hi");
}
