
#include "printer.h"
#include <array>

void basicPrinter::print(std::list<std::list<std::array<std::string, 2>>> _input, std::ostream& _output)
{
	for (auto &i : _input)
	{
		for (auto &j : i)
		{
			_output << std::endl << '<'<< (j.at(0))<< '>' << std::endl;
			_output <<j.at(1) << std::endl;
		}
	}
}
