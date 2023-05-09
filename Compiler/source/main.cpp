#include <iostream>
#include "../include/driver.hpp"
#include "../include/CodeGen.hpp"

std::unique_ptr<CodeGen> GlobalGen = nullptr;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong format of input file" << std::endl;
		return 0;
	}
	yy::Driver program(argv[1]);

	program.parse();
	return 0;
}