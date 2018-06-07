// bpistats.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include "app.h"

int main(int argc, const char** argv)
{
	try
	{
		appForBluzelle theApp;
		theApp.run(argc, argv);
	}
	catch (std::exception &e)
	{
		std::cout << "exception: " << e.what() << std::endl;
	}

	return 0;
}


